#include "JohnnieAnimatorWidget.hpp"

#include "Renderer/Renderer.hpp"
#include "Renderer/RHI/Resources/RHITexture.hpp"

#include "Scene/Entities/Meshes/SkeletalMesh/Animator.hpp"
#include "Log/Log.hpp"

#include "../JohnnieDelegates.hpp"

WJohnnieAnimatorWidget::WJohnnieAnimatorWidget(TSharedPtr<OAnimator> Animator)
	:	m_Animator(MoveTemp(Animator))
{
	InitTextures();
	InitFileBrowser();
	InitAnimatorActions();
}

void WJohnnieAnimatorWidget::OnRenderGui(void)
{
	if (!m_bShouldRenderWidget)
	{
		return;
	}

	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	ImGui::Begin("Animator");
	{
		RenderNavigationBar();
		RenderHorizontalPanel();

		RenderFileBrowser();
	}
	ImGui::End();
}

void WJohnnieAnimatorWidget::OnTick(double DeltaTime)
{
	TSharedPtr<OAnimator> Animator = m_Animator.lock();

	if (Animator)
	{
		m_bShouldRenderWidget = Animator->HasValidSkeletalMesh();
	}
	else
	{
		m_bShouldRenderWidget = false;
	}

	if (m_bShouldRenderWidget)
	{
		UpdateWidget(Animator.get());
	}
}

void WJohnnieAnimatorWidget::InitFileBrowser(void)
{
	m_AnimationFileBrowser.SetTitle("Select Animation");
	m_AnimationFileBrowser.SetTypeFilters({ ".dae" });
}

void WJohnnieAnimatorWidget::InitTextures(void)
{
	RHITexture2DManager& TextureManager = Renderer::Get()->GetTexture2DManager();

	TextureManager.LoadResource("Assets/Textures/ButtonPlay.png");
	TextureManager.LoadResource("Assets/Textures/ButtonPause.png");
	TextureManager.LoadResource("Assets/Textures/ButtonLoadAnimation.png");

	m_PlayButtonTexture = TextureManager.GetResource("Assets/Textures/ButtonPlay.png");
	m_PauseButtonTexture = TextureManager.GetResource("Assets/Textures/ButtonPause.png");
	m_LoadAnimationButtonTexture = TextureManager.GetResource("Assets/Textures/ButtonLoadAnimation.png");
}

void WJohnnieAnimatorWidget::InitAnimatorActions(void)
{
	TSharedPtr<OAnimator> Animator = m_Animator.lock();
	Check(Animator);

	Animator->OnAnimationLoaded.AddLambda(
	[this](uint32_t AnimationIndex) 
	{
		m_SelectedAnimationIndex = AnimationIndex;
	});
}

void WJohnnieAnimatorWidget::RenderNavigationBar(void)
{
	TSharedPtr<OAnimator> Animator = m_Animator.lock();
	Check(Animator);

	const float WindowWidth = ImGui::GetWindowSize().x;

	ImGui::SetCursorPosX((WindowWidth - 32) * 0.5f);

	if (!m_bIsAnimationPlaying)
	{
		if (ImGui::ImageButton((void*)(intptr_t)m_PlayButtonTexture->GetRendererID(), ImVec2(32, 32)))
		{
			OnPlayButtonClicked(Animator.get());
		}
	}
	else
	{
		if (ImGui::ImageButton((void*)(intptr_t)m_PauseButtonTexture->GetRendererID(), ImVec2(32, 32)))
		{
			OnPauseButtonClicked(Animator.get());
		}
	}

	ImGui::SameLine();
	ImGui::SetCursorPosX((WindowWidth - 128) - 50.f);

	if (ImGui::ImageButton((void*)(intptr_t)m_LoadAnimationButtonTexture->GetRendererID(), ImVec2(128, 32)))
	{
		OnLoadAnimationButtonClicked();
	}

	ImGui::SameLine();
	ImGui::SetCursorPosX((WindowWidth - 128) - 30.f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 12.f);

	ImGui::TextUnformatted("Load Animation");
}

void WJohnnieAnimatorWidget::RenderHorizontalPanel(void)
{
	const float WindowWidth = ImGui::GetWindowSize().x;

	ImGui::Dummy(ImVec2(5, 0));

	if (ImGui::BeginTable("DiffuseMapTable", 2, ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_SizingFixedFit))
	{
		ImGui::TableSetupColumn("", 0, 3.5f / 5.f * WindowWidth);
		ImGui::TableSetupColumn("", 0, 1.5f / 5.f * WindowWidth);

		ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
		RenderLeftBox();

		ImGui::TableSetColumnIndex(1);
		RenderRightBox();

		ImGui::EndTable();
	}
}

void WJohnnieAnimatorWidget::RenderLeftBox(void)
{
	TSharedPtr<OAnimator> Animator = m_Animator.lock();
	Check(Animator);

	ImGui::Dummy(ImVec2(0, 23));

	if (ImGui::SliderFloat("Time", &m_CurrentAnimationTimeInSeconds, 0.f,Animator->GetActiveAnimationDurationInSeconds()))
	{
		OnTimelineValueChanged(Animator.get(), m_CurrentAnimationTimeInSeconds);
	}
}

void WJohnnieAnimatorWidget::RenderRightBox(void)
{
	TSharedPtr<OAnimator> Animator = m_Animator.lock();
	Check(Animator);

	ImGui::Dummy(ImVec2(0, 6));
	if (Animator->GetAvailableAnimationsCount() > 0)
	{
		RenderAnimationSelector(Animator.get(),false);
	}
	else
	{
		RenderAnimationSelector(Animator.get(), true);
	}

	ImGui::Dummy(ImVec2(0, 4));

	if (ImGui::Checkbox("Loop", &m_bIsAnimationLooping))
	{
		OnLoopCheckboxClicked(Animator.get(), m_bIsAnimationLooping);
	}
	ImGui::Dummy(ImVec2(0, 6));
}

void WJohnnieAnimatorWidget::RenderAnimationSelector(OAnimator* Animator, bool IsEmpty)
{
	if (IsEmpty)
	{
		ImGui::Combo("Animation", &m_SelectedAnimationIndex, "\0");
		return;
	}

	struct ComboOption
	{
		std::string Name;
		bool IsSelected{ false };

		ComboOption(std::string_view Name) 
			:	Name(Name)
		{}
	};
	std::vector<ComboOption> Options;

	for (uint16_t idx = 0; idx < Animator->GetAvailableAnimationsNames().size(); idx++)
	{
		std::string_view Name = Animator->GetAvailableAnimationsNames()[idx];
		Options.emplace_back(Name);
	}

	if(ImGui::BeginCombo("Animation", Options[m_SelectedAnimationIndex].Name.data()))
	{ 
		for (uint8_t idx = 0; idx < Options.size() ;idx++)
		{
			if (ImGui::Selectable(Options[idx].Name.c_str(), Options[idx].IsSelected))
			{
				m_SelectedAnimationIndex = idx;
				OnSelectedAnimationChanged(Animator, Options[idx].Name);
			}
		}
		ImGui::EndCombo();
	}
}

void WJohnnieAnimatorWidget::RenderFileBrowser(void)
{
	m_AnimationFileBrowser.Display();
	
	if (m_AnimationFileBrowser.HasSelected())
	{
		std::string FilePath = m_AnimationFileBrowser.GetSelected().string();
		OnFileBrowserSelectionConfirmed(FilePath);
	}
}

void WJohnnieAnimatorWidget::UpdateWidget(OAnimator* Animator)
{
	m_bIsAnimationPlaying = Animator->IsAnimationActive() && !Animator->IsPaused();
	m_bIsAnimationLooping = Animator->IsLooping();
	m_CurrentAnimationTimeInSeconds = Animator->GetCurrentAnimationTimeInSeconds();
}

void WJohnnieAnimatorWidget::OnPlayButtonClicked(OAnimator* Animator)
{
	Animator->Resume();
}

void WJohnnieAnimatorWidget::OnPauseButtonClicked(OAnimator* Animator)
{
	Animator->Pause();
}

void WJohnnieAnimatorWidget::OnLoopCheckboxClicked(OAnimator* Animator, bool Loop)
{
	Animator->SetLoop(Loop);
}

void WJohnnieAnimatorWidget::OnSelectedAnimationChanged(OAnimator* Animator, std::string const& NewAnimationName)
{
	Animator->SetActiveAnimationName(NewAnimationName);
}

void WJohnnieAnimatorWidget::OnTimelineValueChanged(OAnimator* Animator, float NewValue)
{
	Animator->SetAnimationTime(m_CurrentAnimationTimeInSeconds, true);
}

void WJohnnieAnimatorWidget::OnLoadAnimationButtonClicked(void)
{
	m_AnimationFileBrowser.Open();
}

void WJohnnieAnimatorWidget::OnFileBrowserSelectionConfirmed(std::string const& FilePath)
{
	LOG(Core, Trace, "Loading animation from {0}", FilePath);
	JohnnieDelegates::Get()->OnAnimationToLoadPathSelected.Broadcast(FilePath);

	m_AnimationFileBrowser.ClearSelected();
}

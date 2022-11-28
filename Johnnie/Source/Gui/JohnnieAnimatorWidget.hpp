#pragma once

#include <Engine/Gui.hpp>
#include <glm/glm.hpp>

namespace Engine
{
	class OAnimator;

	namespace RHI
	{
		class RHITexture2D;
	}
}

//struct ComboOption
//{
//	std::string Name;
//	bool IsSelected{ false };
//
//	explicit ComboOption(std::string_view Name)
//		: Name(Name)
//	{}
//};

class WJohnnieAnimatorWidget final : public ImGuiWidgetBase
{
private:
	ImGui::FileBrowser m_AnimationFileBrowser{};

	TWeakPtr<OAnimator> m_Animator;

	TSharedPtr<RHITexture2D> m_PlayButtonTexture;
	TSharedPtr<RHITexture2D> m_PauseButtonTexture;
	TSharedPtr<RHITexture2D> m_LoadAnimationButtonTexture;

	bool m_bShouldRenderWidget{ false };
	bool m_bIsAnimationPlaying{ false };
	bool m_bIsAnimationLooping{ false };

	float m_CurrentAnimationTimeInSeconds = 0.f;
	int32_t m_SelectedAnimationIndex = 0;

	//std::vector<ComboOption> m_Options;

public:
	WJohnnieAnimatorWidget(TSharedPtr<OAnimator> Animator);

	virtual void OnRenderGui(void) override;
	virtual void OnTick(double DeltaTime) override;

private:
	void InitFileBrowser(void);
	void InitTextures(void);
	void InitAnimatorActions(void);

	void RenderNavigationBar(void);
	void RenderHorizontalPanel(void);
	void RenderLeftBox(void);
	void RenderRightBox(void);
	void RenderAnimationSelector(OAnimator* Animator, bool IsEmpty);

	void RenderFileBrowser(void);
	
	void UpdateWidget(OAnimator* Animator);

private:
	void OnPlayButtonClicked(OAnimator* Animator);
	void OnPauseButtonClicked(OAnimator* Animator);
	void OnLoopCheckboxClicked(OAnimator* Animator, bool Loop);
	void OnSelectedAnimationChanged(OAnimator* Animator, std::string const& NewAnimationName);
	void OnTimelineValueChanged(OAnimator* Animator, float NewValue);
	void OnLoadAnimationButtonClicked(void);
	void OnFileBrowserSelectionConfirmed(std::string const& FilePath);
};
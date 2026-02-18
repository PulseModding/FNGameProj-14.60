#include "AsherUtils.h"
#include "AsherUtilsStyle.h"
#include "AsherUtilsCommands.h"
#include "SCharacterCreationWindow.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Framework/Application/SlateApplication.h"
#include "ToolMenus.h"

static const FName AsherUtilsTabName("AsherUtils");

#define LOCTEXT_NAMESPACE "FAsherUtilsModule"

void FAsherUtilsModule::StartupModule()
{
	FAsherUtilsStyle::Initialize();
	FAsherUtilsStyle::ReloadTextures();

	FAsherUtilsCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FAsherUtilsCommands::Get().CreateCharacterDataAsset,
		FExecuteAction::CreateRaw(this, &FAsherUtilsModule::OnCreateCharacterDataAssetClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FAsherUtilsModule::RegisterMenus));
}

void FAsherUtilsModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	FAsherUtilsStyle::Shutdown();
	FAsherUtilsCommands::Unregister();
}

void FAsherUtilsModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	// Register toolbar extension in UE4
	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FAsherUtilsCommands::Get().CreateCharacterDataAsset));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}

	// Add to Window menu in UE4
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("GetContent");
			Section.AddMenuEntryWithCommandList(FAsherUtilsCommands::Get().CreateCharacterDataAsset, PluginCommands);
		}
	}
	
	// Add to Content Browser context menu
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("ContentBrowser.AddNewContextMenu");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("ContentBrowserNewAdvancedAsset");
			Section.AddMenuEntryWithCommandList(FAsherUtilsCommands::Get().CreateCharacterDataAsset, PluginCommands);
		}
	}
}

void FAsherUtilsModule::OnCreateCharacterDataAssetClicked()
{
	TSharedRef<SWindow> Window = SNew(SWindow)
		.Title(LOCTEXT("CharacterCreationWindowTitle", "Create Character Data Asset"))
		.ClientSize(FVector2D(500, 700))
		.SupportsMaximize(false)
		.SupportsMinimize(false);

	TSharedRef<SCharacterCreationWindow> CharacterCreationWindow = SNew(SCharacterCreationWindow);
	
	// Store parent window reference
	const_cast<SCharacterCreationWindow&>(CharacterCreationWindow.Get()).ParentWindow = Window;

	Window->SetContent(CharacterCreationWindow);

	FSlateApplication::Get().AddWindow(Window);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAsherUtilsModule, AsherUtils)

#include "SCharacterCreationWindow.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Layout/SSeparator.h"
#include "EditorStyleSet.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "AthenaCharacterItemDefinition.h"
#include "FortHeroType.h"
#include "FortHeroSpecialization.h"
#include "CustomCharacterPart.h"
#include "AssetRegistryModule.h"
#include "Misc/MessageDialog.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "Misc/PackageName.h"
#include "UObject/Package.h"
#include "FileHelpers.h"
#include "Misc/ConfigCacheIni.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/Texture2D.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimBlueprint.h"
#include "Animation/AnimMontage.h"
#include "Misc/Paths.h"

#define LOCTEXT_NAMESPACE "SCharacterCreationWindow"

void SCharacterCreationWindow::Construct(const FArguments& InArgs)
{
	// Initialize defaults
	SkinCodeName = TEXT("CharacterIDHere");
	DisplayName = TEXT("Get from fn.gg or fmodel");
	Description = TEXT("new cid");
	Gender = EFortCustomGender::Both;
	BodyType = EFortCustomBodyType::All;
	NotifyType = TEXT("");
	
	SkelMeshBodyPath = TEXT("");
	SkelMeshHeadPath = TEXT("");
	SkelMeshFaceAccPath = TEXT("");
	IconLPath = TEXT("");
	IconPath = TEXT("");
	ABPBodyPath = TEXT("");
	ABPHeadPath = TEXT("");
	ABPFaceAccPath = TEXT("");
	BaseHeroTypePath = TEXT("");
	BaseHSPath = TEXT("");
	BaseCPPath = TEXT("");
	BaseCIDPath = TEXT("");
	FrontEndItemMontagePath = TEXT("");
	
	// Setup Gender options
	GenderOptions.Add(MakeShareable(new FString("Male")));
	GenderOptions.Add(MakeShareable(new FString("Female")));
	GenderOptions.Add(MakeShareable(new FString("Both")));
	SelectedGender = GenderOptions[2]; // Default to Both
	
	// Setup Body Type options
	BodyTypeOptions.Add(MakeShareable(new FString("Small")));
	BodyTypeOptions.Add(MakeShareable(new FString("Medium")));
	BodyTypeOptions.Add(MakeShareable(new FString("Large")));
	BodyTypeOptions.Add(MakeShareable(new FString("All")));
	SelectedBodyType = BodyTypeOptions[3]; // Default to All
	
	LoadSettings();
	
	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
		.Padding(8.0f)
		[
			SNew(SVerticalBox)
			
			// Title
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 0, 0, 10)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("WindowTitle", "Create Character"))
				.Font(FEditorStyle::GetFontStyle("HeadingExtraSmall"))
			]
			
			// Scrollable content
			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SNew(SScrollBox)
				
				// Skin Code Name
				+ SScrollBox::Slot()
				.Padding(0, 2)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0, 0, 10, 0)
					[
						SNew(SBox)
						.WidthOverride(120)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("SkinCodeName", "CID"))
						]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(SEditableTextBox)
						.Text(FText::FromString(SkinCodeName))
						.OnTextChanged(this, &SCharacterCreationWindow::OnSkinCodeNameChanged)
					]
				]
				
				// Display Name
				+ SScrollBox::Slot()
				.Padding(0, 2)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0, 0, 10, 0)
					[
						SNew(SBox)
						.WidthOverride(120)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("DisplayName", "Display Name"))
						]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(SEditableTextBox)
						.Text(FText::FromString(DisplayName))
						.OnTextChanged(this, &SCharacterCreationWindow::OnDisplayNameChanged)
					]
				]
				
				// Description
				+ SScrollBox::Slot()
				.Padding(0, 2)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0, 0, 10, 0)
					[
						SNew(SBox)
						.WidthOverride(120)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("Description", "Description"))
						]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(SEditableTextBox)
						.Text(FText::FromString(Description))
						.OnTextChanged(this, &SCharacterCreationWindow::OnDescriptionChanged)
					]
				]
				
				// Gender
				+ SScrollBox::Slot()
				.Padding(0, 2)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0, 0, 10, 0)
					[
						SNew(SBox)
						.WidthOverride(120)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("Gender", "Gender"))
						]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(SComboBox<TSharedPtr<FString>>)
						.OptionsSource(&GenderOptions)
						.OnGenerateWidget_Lambda([](TSharedPtr<FString> Item)
						{
							return SNew(STextBlock).Text(FText::FromString(*Item));
						})
						.OnSelectionChanged(this, &SCharacterCreationWindow::OnGenderChanged)
						.Content()
						[
							SNew(STextBlock)
							.Text_Lambda([this]() { return FText::FromString(*SelectedGender); })
						]
					]
				]
				
				// Body Type
				+ SScrollBox::Slot()
				.Padding(0, 2)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0, 0, 10, 0)
					[
						SNew(SBox)
						.WidthOverride(120)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("BodyType", "Body Type"))
						]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(SComboBox<TSharedPtr<FString>>)
						.OptionsSource(&BodyTypeOptions)
						.OnGenerateWidget_Lambda([](TSharedPtr<FString> Item)
						{
							return SNew(STextBlock).Text(FText::FromString(*Item));
						})
						.OnSelectionChanged(this, &SCharacterCreationWindow::OnBodyTypeChanged)
						.Content()
						[
							SNew(STextBlock)
							.Text_Lambda([this]() { return FText::FromString(*SelectedBodyType); })
						]
					]
				]
				
				// Notify Type
					/*
									+ SScrollBox::Slot()
				.Padding(0, 2)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0, 0, 10, 0)
					[
						SNew(SBox)
						.WidthOverride(120)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("NotifyType", "Notify Type"))
						]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(SEditableTextBox)
						.Text(FText::FromString(NotifyType))
						.OnTextChanged(this, &SCharacterCreationWindow::OnNotifyTypeChanged)
					]
				]
				*/

				
				// Separator
				+ SScrollBox::Slot()
				.Padding(0, 10)
				[
					SNew(SSeparator)
				]
				
				// Skel Mesh - Body
				+ SScrollBox::Slot()
				.Padding(0, 2)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0, 0, 10, 0)
					[
						SNew(SBox)
						.WidthOverride(120)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("SkelMeshBody", "Skel Mesh - Body"))
						]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(STextBlock)
						.Text_Lambda([this]() { 
							return FText::FromString(SkelMeshBodyPath.IsEmpty() ? TEXT("None") : FPaths::GetBaseFilename(SkelMeshBodyPath)); 
						})
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(5, 0, 0, 0)
					[
						SNew(SButton)
						.Text(LOCTEXT("Browse", "Browse"))
						.OnClicked_Lambda([this]() {
							BrowseForAsset(SkelMeshBodyPath, USkeletalMesh::StaticClass());
							return FReply::Handled();
						})
					]
				]
				
				// Skel Mesh - Head
				+ SScrollBox::Slot()
				.Padding(0, 2)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0, 0, 10, 0)
					[
						SNew(SBox)
						.WidthOverride(120)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("SkelMeshHead", "Skel Mesh - Head"))
						]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(STextBlock)
						.Text_Lambda([this]() { 
							return FText::FromString(SkelMeshHeadPath.IsEmpty() ? TEXT("None") : FPaths::GetBaseFilename(SkelMeshHeadPath)); 
						})
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(5, 0, 0, 0)
					[
						SNew(SButton)
						.Text(LOCTEXT("Browse", "Browse"))
						.OnClicked_Lambda([this]() {
							BrowseForAsset(SkelMeshHeadPath, USkeletalMesh::StaticClass());
							return FReply::Handled();
						})
					]
				]
				
				// Skel Mesh - Face Acc
				+ SScrollBox::Slot()
				.Padding(0, 2)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0, 0, 10, 0)
					[
						SNew(SBox)
						.WidthOverride(120)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("SkelMeshFaceAcc", "Skel Mesh - Face Acc"))
						]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(STextBlock)
						.Text_Lambda([this]() { 
							return FText::FromString(SkelMeshFaceAccPath.IsEmpty() ? TEXT("None") : FPaths::GetBaseFilename(SkelMeshFaceAccPath)); 
						})
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(5, 0, 0, 0)
					[
						SNew(SButton)
						.Text(LOCTEXT("Browse", "Browse"))
						.OnClicked_Lambda([this]() {
							BrowseForAsset(SkelMeshFaceAccPath, USkeletalMesh::StaticClass());
							return FReply::Handled();
						})
					]
				]
				
				// Icon L
				+ SScrollBox::Slot()
				.Padding(0, 2)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0, 0, 10, 0)
					[
						SNew(SBox)
						.WidthOverride(120)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("IconL", "Icon L"))
						]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(STextBlock)
						.Text_Lambda([this]() { 
							return FText::FromString(IconLPath.IsEmpty() ? TEXT("None") : FPaths::GetBaseFilename(IconLPath)); 
						})
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(5, 0, 0, 0)
					[
						SNew(SButton)
						.Text(LOCTEXT("Browse", "Browse"))
						.OnClicked_Lambda([this]() {
							BrowseForAsset(IconLPath, UTexture2D::StaticClass());
							return FReply::Handled();
						})
					]
				]
				
				// Icon
				+ SScrollBox::Slot()
				.Padding(0, 2)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0, 0, 10, 0)
					[
						SNew(SBox)
						.WidthOverride(120)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("Icon", "Icon"))
						]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(STextBlock)
						.Text_Lambda([this]() { 
							return FText::FromString(IconPath.IsEmpty() ? TEXT("None") : FPaths::GetBaseFilename(IconPath)); 
						})
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(5, 0, 0, 0)
					[
						SNew(SButton)
						.Text(LOCTEXT("Browse", "Browse"))
						.OnClicked_Lambda([this]() {
							BrowseForAsset(IconPath, UTexture2D::StaticClass());
							return FReply::Handled();
						})
					]
				]
				
				// ABP - Body
				+ SScrollBox::Slot()
				.Padding(0, 2)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0, 0, 10, 0)
					[
						SNew(SBox)
						.WidthOverride(120)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("ABPBody", "ABP - Body"))
						]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(STextBlock)
						.Text_Lambda([this]() { 
							return FText::FromString(ABPBodyPath.IsEmpty() ? TEXT("None") : FPaths::GetBaseFilename(ABPBodyPath)); 
						})
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(5, 0, 0, 0)
					[
						SNew(SButton)
						.Text(LOCTEXT("Browse", "Browse"))
						.OnClicked_Lambda([this]() {
							BrowseForAsset(ABPBodyPath, UAnimBlueprint::StaticClass());
							return FReply::Handled();
						})
					]
				]
				
				// ABP - Head
				+ SScrollBox::Slot()
				.Padding(0, 2)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0, 0, 10, 0)
					[
						SNew(SBox)
						.WidthOverride(120)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("ABPHead", "ABP - Head"))
						]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(STextBlock)
						.Text_Lambda([this]() { 
							return FText::FromString(ABPHeadPath.IsEmpty() ? TEXT("None") : FPaths::GetBaseFilename(ABPHeadPath)); 
						})
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(5, 0, 0, 0)
					[
						SNew(SButton)
						.Text(LOCTEXT("Browse", "Browse"))
						.OnClicked_Lambda([this]() {
							BrowseForAsset(ABPHeadPath, UAnimBlueprint::StaticClass());
							return FReply::Handled();
						})
					]
				]
				
				// ABP - Face Acc
				+ SScrollBox::Slot()
				.Padding(0, 2)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0, 0, 10, 0)
					[
						SNew(SBox)
						.WidthOverride(120)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("ABPFaceAcc", "ABP - Face Acc"))
						]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(STextBlock)
						.Text_Lambda([this]() { 
							return FText::FromString(ABPFaceAccPath.IsEmpty() ? TEXT("None") : FPaths::GetBaseFilename(ABPFaceAccPath)); 
						})
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(5, 0, 0, 0)
					[
						SNew(SButton)
						.Text(LOCTEXT("Browse", "Browse"))
						.OnClicked_Lambda([this]() {
							BrowseForAsset(ABPFaceAccPath, UAnimBlueprint::StaticClass());
							return FReply::Handled();
						})
					]
				]
				
				// Separator
				+ SScrollBox::Slot()
				.Padding(0, 10)
				[
					SNew(SSeparator)
				]
				
				// Base Hero Type
				+ SScrollBox::Slot()
				.Padding(0, 2)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0, 0, 10, 0)
					[
						SNew(SBox)
						.WidthOverride(120)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("BaseHeroType", "Base Hero Type"))
						]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(STextBlock)
						.Text_Lambda([this]() { 
							return FText::FromString(BaseHeroTypePath.IsEmpty() ? TEXT("None") : FPaths::GetBaseFilename(BaseHeroTypePath)); 
						})
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(5, 0, 0, 0)
					[
						SNew(SButton)
						.Text(LOCTEXT("Browse", "Browse"))
						.OnClicked_Lambda([this]() {
							BrowseForAsset(BaseHeroTypePath, UFortHeroType::StaticClass());
							return FReply::Handled();
						})
					]
				]
				
				// Base HS
				+ SScrollBox::Slot()
				.Padding(0, 2)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0, 0, 10, 0)
					[
						SNew(SBox)
						.WidthOverride(120)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("BaseHS", "Base HS"))
						]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(STextBlock)
						.Text_Lambda([this]() { 
							return FText::FromString(BaseHSPath.IsEmpty() ? TEXT("None") : FPaths::GetBaseFilename(BaseHSPath)); 
						})
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(5, 0, 0, 0)
					[
						SNew(SButton)
						.Text(LOCTEXT("Browse", "Browse"))
						.OnClicked_Lambda([this]() {
							BrowseForAsset(BaseHSPath, UFortHeroSpecialization::StaticClass());
							return FReply::Handled();
						})
					]
				]
				
				// Base CP
				+ SScrollBox::Slot()
				.Padding(0, 2)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0, 0, 10, 0)
					[
						SNew(SBox)
						.WidthOverride(120)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("BaseCP", "Base CP"))
						]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(STextBlock)
						.Text_Lambda([this]() { 
							return FText::FromString(BaseCPPath.IsEmpty() ? TEXT("None") : FPaths::GetBaseFilename(BaseCPPath)); 
						})
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(5, 0, 0, 0)
					[
						SNew(SButton)
						.Text(LOCTEXT("Browse", "Browse"))
						.OnClicked_Lambda([this]() {
							BrowseForAsset(BaseCPPath, UCustomCharacterPart::StaticClass());
							return FReply::Handled();
						})
					]
				]
				
				// Base CID
				+ SScrollBox::Slot()
				.Padding(0, 2)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0, 0, 10, 0)
					[
						SNew(SBox)
						.WidthOverride(120)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("BaseCID", "Base CID"))
						]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(STextBlock)
						.Text_Lambda([this]() { 
							return FText::FromString(BaseCIDPath.IsEmpty() ? TEXT("None") : FPaths::GetBaseFilename(BaseCIDPath)); 
						})
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(5, 0, 0, 0)
					[
						SNew(SButton)
						.Text(LOCTEXT("Browse", "Browse"))
						.OnClicked_Lambda([this]() {
							BrowseForAsset(BaseCIDPath, UAthenaCharacterItemDefinition::StaticClass());
							return FReply::Handled();
						})
					]
				]
				
				// FrontEnd Item Montage
				+ SScrollBox::Slot()
				.Padding(0, 2)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0, 0, 10, 0)
					[
						SNew(SBox)
						.WidthOverride(120)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("FrontEndMontage", "FrontEnd Montage"))
						]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(STextBlock)
						.Text_Lambda([this]() { 
							return FText::FromString(FrontEndItemMontagePath.IsEmpty() ? TEXT("None") : FPaths::GetBaseFilename(FrontEndItemMontagePath)); 
						})
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(5, 0, 0, 0)
					[
						SNew(SButton)
						.Text(LOCTEXT("Browse", "Browse"))
						.OnClicked_Lambda([this]() {
							BrowseForAsset(FrontEndItemMontagePath, UAnimMontage::StaticClass());
							return FReply::Handled();
						})
					]
				]
				
				// Separator
				+ SScrollBox::Slot()
				.Padding(0, 10)
				[
					SNew(SSeparator)
				]
			]
			
			// Buttons
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 10, 0, 0)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(0, 0, 5, 0)
				[
					SNew(SButton)
					.Text(LOCTEXT("OK", "OK"))
					.OnClicked(this, &SCharacterCreationWindow::OnCreateCharacterClicked)
				]
				
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.Text(LOCTEXT("Cancel", "Cancel"))
					.OnClicked(this, &SCharacterCreationWindow::OnCancelClicked)
				]
			]
		]
	];
}

void SCharacterCreationWindow::OnSkinCodeNameChanged(const FText& NewText)
{
	SkinCodeName = NewText.ToString();
}

void SCharacterCreationWindow::OnDisplayNameChanged(const FText& NewText)
{
	DisplayName = NewText.ToString();
}

void SCharacterCreationWindow::OnDescriptionChanged(const FText& NewText)
{
	Description = NewText.ToString();
}

void SCharacterCreationWindow::OnGenderChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo)
{
	SelectedGender = NewValue;
	if (*NewValue == "Male")
		Gender = EFortCustomGender::Male;
	else if (*NewValue == "Female")
		Gender = EFortCustomGender::Female;
	else
		Gender = EFortCustomGender::Both;
}

void SCharacterCreationWindow::OnBodyTypeChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo)
{
	SelectedBodyType = NewValue;
	if (*NewValue == "Small")
		BodyType = EFortCustomBodyType::Small;
	else if (*NewValue == "Medium")
		BodyType = EFortCustomBodyType::Medium;
	else if (*NewValue == "Large")
		BodyType = EFortCustomBodyType::Large;
	else
		BodyType = EFortCustomBodyType::All;
}

void SCharacterCreationWindow::OnNotifyTypeChanged(const FText& NewText)
{
	NotifyType = NewText.ToString();
}

FReply SCharacterCreationWindow::OnCreateCharacterClicked()
{
	SaveSettings();
	CreateCharacterAssets();
	
	if (ParentWindow.IsValid())
	{
		ParentWindow->RequestDestroyWindow();
	}
	
	return FReply::Handled();
}

FReply SCharacterCreationWindow::OnCancelClicked()
{
	if (ParentWindow.IsValid())
	{
		ParentWindow->RequestDestroyWindow();
	}
	
	return FReply::Handled();
}

void SCharacterCreationWindow::LoadSettings()
{
	GConfig->GetString(TEXT("AsherUtils"), TEXT("SkinCodeName"), SkinCodeName, GEditorPerProjectIni);
	GConfig->GetString(TEXT("AsherUtils"), TEXT("DisplayName"), DisplayName, GEditorPerProjectIni);
	GConfig->GetString(TEXT("AsherUtils"), TEXT("Description"), Description, GEditorPerProjectIni);
	// ONLY load base asset paths - mesh/ABP/icon paths reset each session
	GConfig->GetString(TEXT("AsherUtils"), TEXT("BaseHeroTypePath"), BaseHeroTypePath, GEditorPerProjectIni);
	GConfig->GetString(TEXT("AsherUtils"), TEXT("BaseHSPath"), BaseHSPath, GEditorPerProjectIni);
	GConfig->GetString(TEXT("AsherUtils"), TEXT("BaseCPPath"), BaseCPPath, GEditorPerProjectIni);
	GConfig->GetString(TEXT("AsherUtils"), TEXT("BaseCIDPath"), BaseCIDPath, GEditorPerProjectIni);
	GConfig->GetString(TEXT("AsherUtils"), TEXT("FrontEndItemMontagePath"), FrontEndItemMontagePath, GEditorPerProjectIni);
}

void SCharacterCreationWindow::SaveSettings()
{
	GConfig->SetString(TEXT("AsherUtils"), TEXT("SkinCodeName"), *SkinCodeName, GEditorPerProjectIni);
	GConfig->SetString(TEXT("AsherUtils"), TEXT("DisplayName"), *DisplayName, GEditorPerProjectIni);
	GConfig->SetString(TEXT("AsherUtils"), TEXT("Description"), *Description, GEditorPerProjectIni);
	// ONLY save base asset paths - mesh/ABP/icon paths do NOT persist
	GConfig->SetString(TEXT("AsherUtils"), TEXT("BaseHeroTypePath"), *BaseHeroTypePath, GEditorPerProjectIni);
	GConfig->SetString(TEXT("AsherUtils"), TEXT("BaseHSPath"), *BaseHSPath, GEditorPerProjectIni);
	GConfig->SetString(TEXT("AsherUtils"), TEXT("BaseCPPath"), *BaseCPPath, GEditorPerProjectIni);
	GConfig->SetString(TEXT("AsherUtils"), TEXT("BaseCIDPath"), *BaseCIDPath, GEditorPerProjectIni);
	GConfig->SetString(TEXT("AsherUtils"), TEXT("FrontEndItemMontagePath"), *FrontEndItemMontagePath, GEditorPerProjectIni);
	GConfig->Flush(false, GEditorPerProjectIni);
}

void SCharacterCreationWindow::BrowseForAsset(FString& OutPath, UClass* AssetClass)
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	
	FAssetPickerConfig AssetPickerConfig;
	AssetPickerConfig.SelectionMode = ESelectionMode::Single;
	AssetPickerConfig.InitialAssetViewType = EAssetViewType::List;
	AssetPickerConfig.bFocusSearchBoxWhenOpened = true;
	AssetPickerConfig.bAllowNullSelection = false;
	AssetPickerConfig.bShowBottomToolbar = true;
	AssetPickerConfig.bAutohideSearchBar = false;
	AssetPickerConfig.bAllowDragging = false;
	AssetPickerConfig.bCanShowClasses = false;
	AssetPickerConfig.bShowPathInColumnView = true;
	AssetPickerConfig.bShowTypeInColumnView = false;
	AssetPickerConfig.bSortByPathInColumnView = false;
	
	if (AssetClass)
	{
		AssetPickerConfig.Filter.ClassNames.Add(AssetClass->GetFName());
	}
	
	AssetPickerConfig.OnAssetSelected = FOnAssetSelected::CreateLambda([&OutPath, AssetClass](const FAssetData& AssetData) {
		if (AssetClass && AssetClass->GetName() == TEXT("AnimBlueprint"))
		{
			OutPath = AssetData.ObjectPath.ToString() + TEXT("_C");
		}
		else
		{
			OutPath = AssetData.ObjectPath.ToString();
		}
		FSlateApplication::Get().DismissAllMenus();
	});
	
	TSharedRef<SWidget> AssetPickerWidget = ContentBrowserModule.Get().CreateAssetPicker(AssetPickerConfig);
	
	FSlateApplication::Get().PushMenu(
		ParentWindow.ToSharedRef(),
		FWidgetPath(),
		AssetPickerWidget,
		FSlateApplication::Get().GetCursorPos(),
		FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu)
	);
}

void SCharacterCreationWindow::CreateCharacterAssets()
{
	if (SkinCodeName.IsEmpty())
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("EmptyName", "CID cannot be empty!"));
		return;
	}
	
	// Determine gender folder
	FString GenderFolder;
	if (Gender == EFortCustomGender::Male)
		GenderFolder = TEXT("Male");
	else if (Gender == EFortCustomGender::Female)
		GenderFolder = TEXT("Female");
	else
		GenderFolder = TEXT("Both");
	
	// Folder structure: Parts in /Game/Surge/Characters/{Gender}/{ID}/CharacterParts/
	// CID in /Game/Athena/Items/Cosmetics/Characters/
	FString SurgePath = FString::Printf(TEXT("/Game/Asher/Characters/%s/%s"), *GenderFolder, *SkinCodeName);
	FString PartsPath = SurgePath / TEXT("CharacterParts");
	FString CharacterDefPath = TEXT("/York/Content/Items/Cosmetics/Characters");
	
	TArray<UObject*> CreatedAssets;
	
	// Create Character Parts (Head, Body, Face) - with optional base CP duplication
	TArray<UCustomCharacterPart*> CharacterParts;
	
	struct FPartInfo
	{
		FString Name;
		EFortCustomPartType Type;
		FString MeshPath;
		FString ABPPath;
	};
	
	TArray<FPartInfo> Parts = {
		{ TEXT("Head"), EFortCustomPartType::Head, SkelMeshHeadPath, ABPHeadPath },
		{ TEXT("Body"), EFortCustomPartType::Body, SkelMeshBodyPath, ABPBodyPath },
		{ TEXT("Face"), EFortCustomPartType::Face, SkelMeshFaceAccPath, ABPFaceAccPath }
	};
	
	// Load base CP if provided
	UCustomCharacterPart* BaseCP = nullptr;
	if (!BaseCPPath.IsEmpty())
	{
		BaseCP = LoadObject<UCustomCharacterPart>(nullptr, *BaseCPPath);
	}
	
	for (const FPartInfo& PartInfo : Parts)
	{
		FString PartName = FString::Printf(TEXT("CP_%s_%s"), *SkinCodeName, *PartInfo.Name);
		FString PackageName = PartsPath / PartName;
		
		UPackage* Package = CreatePackage(nullptr, *PackageName);
		UCustomCharacterPart* CharacterPart = nullptr;
		
		// Duplicate from base if available, otherwise create new
		if (BaseCP)
		{
			CharacterPart = DuplicateObject<UCustomCharacterPart>(BaseCP, Package, *PartName);
			if (CharacterPart)
			{
				CharacterPart->SetFlags(RF_Public | RF_Standalone);
				// Update part-specific properties
				CharacterPart->CharacterPartType = PartInfo.Type;
			}
		}
		else
		{
			CharacterPart = NewObject<UCustomCharacterPart>(Package, *PartName, RF_Public | RF_Standalone);
		}
		
		if (CharacterPart)
		{
			CharacterPart->CharacterPartType = PartInfo.Type;
			CharacterPart->GenderPermitted = Gender;
			CharacterPart->BodyTypesPermitted = BodyType;
			
			// Set skeletal mesh if provided using reflection
			if (!PartInfo.MeshPath.IsEmpty())
			{
				USkeletalMesh* Mesh = LoadObject<USkeletalMesh>(nullptr, *PartInfo.MeshPath);
				if (Mesh)
				{
					FProperty* SkeletalMeshProperty = CharacterPart->GetClass()->FindPropertyByName(FName("SkeletalMesh"));
					if (SkeletalMeshProperty)
					{
						TSoftObjectPtr<USkeletalMesh>* SkeletalMeshPtr = SkeletalMeshProperty->ContainerPtrToValuePtr<TSoftObjectPtr<USkeletalMesh>>(CharacterPart);
						if (SkeletalMeshPtr)
						{
							*SkeletalMeshPtr = TSoftObjectPtr<USkeletalMesh>(Mesh);
						}
					}
				}
			}
			
			// Initialize AdditionalData based on part type
#if WITH_EDITOR
			FPropertyChangedEvent PropChangeEvent(nullptr);
			CharacterPart->PostEditChangeProperty(PropChangeEvent);
			
			// Set ABP if provided and AdditionalData supports it using reflection
			if (!PartInfo.ABPPath.IsEmpty())
			{
				UClass* ABPClass = LoadObject<UClass>(nullptr, *PartInfo.ABPPath);
				if (ABPClass)
				{
					FProperty* AdditionalDataProperty = CharacterPart->GetClass()->FindPropertyByName(FName("AdditionalData"));
					if (AdditionalDataProperty)
					{
						UObject** AdditionalDataPtr = AdditionalDataProperty->ContainerPtrToValuePtr<UObject*>(CharacterPart);
						if (AdditionalDataPtr && *AdditionalDataPtr)
						{
							UObject* AdditionalData = *AdditionalDataPtr;
							
							// Try to find AnimClass property on the AdditionalData object
							FProperty* AnimClassProperty = AdditionalData->GetClass()->FindPropertyByName(FName("AnimClass"));
							if (AnimClassProperty)
							{
								TSoftClassPtr<UAnimInstance>* AnimClassPtr = AnimClassProperty->ContainerPtrToValuePtr<TSoftClassPtr<UAnimInstance>>(AdditionalData);
								if (AnimClassPtr)
								{
									*AnimClassPtr = TSoftClassPtr<UAnimInstance>(ABPClass);
								}
							}
						}
					}
				}
			}
#endif
			
			// Clear MasterSkeletalMesh from Head and FaceAcc parts ONLY
			if (PartInfo.Type == EFortCustomPartType::Head || PartInfo.Type == EFortCustomPartType::Face)
			{
				FProperty* AdditionalDataProperty = CharacterPart->GetClass()->FindPropertyByName(FName("AdditionalData"));
				if (AdditionalDataProperty)
				{
					UObject** AdditionalDataPtr = AdditionalDataProperty->ContainerPtrToValuePtr<UObject*>(CharacterPart);
					if (AdditionalDataPtr && *AdditionalDataPtr)
					{
						UObject* AdditionalData = *AdditionalDataPtr;
						
						// Clear MasterSkeletalMesh
						FProperty* MasterSkeletalMeshProperty = AdditionalData->GetClass()->FindPropertyByName(FName("MasterSkeletalMesh"));
						if (MasterSkeletalMeshProperty)
						{
							TSoftObjectPtr<USkeletalMesh>* MasterSkeletalMeshPtr = MasterSkeletalMeshProperty->ContainerPtrToValuePtr<TSoftObjectPtr<USkeletalMesh>>(AdditionalData);
							if (MasterSkeletalMeshPtr)
							{
								*MasterSkeletalMeshPtr = nullptr;
							}
						}
					}
				}
			}
			
			FAssetRegistryModule::AssetCreated(CharacterPart);
			Package->MarkPackageDirty();
			
			CharacterParts.Add(CharacterPart);
			CreatedAssets.Add(CharacterPart);
		}
	}
	
	// Create Hero Specialization at /Game/Surge/Characters/{Gender}/{ID}/HS_{ID}
	FString SpecializationName = FString::Printf(TEXT("HS_%s"), *SkinCodeName);
	FString SpecializationPackageName = SurgePath / SpecializationName;
	
	UPackage* SpecializationPackage = CreatePackage(nullptr, *SpecializationPackageName);
	UFortHeroSpecialization* HeroSpecialization = nullptr;
	
	// Duplicate from base if available
	if (!BaseHSPath.IsEmpty())
	{
		UFortHeroSpecialization* BaseHS = LoadObject<UFortHeroSpecialization>(nullptr, *BaseHSPath);
		if (BaseHS)
		{
			HeroSpecialization = DuplicateObject<UFortHeroSpecialization>(BaseHS, SpecializationPackage, *SpecializationName);
			if (HeroSpecialization)
			{
				HeroSpecialization->SetFlags(RF_Public | RF_Standalone);
			}
		}
	}
	
	if (!HeroSpecialization)
	{
		HeroSpecialization = NewObject<UFortHeroSpecialization>(SpecializationPackage, *SpecializationName, RF_Public | RF_Standalone);
	}
	
	if (HeroSpecialization)
	{
		// Use reflection to set private CharacterParts array
		FProperty* CharacterPartsProperty = HeroSpecialization->GetClass()->FindPropertyByName(FName("CharacterParts"));
		if (CharacterPartsProperty)
		{
			TArray<TSoftObjectPtr<UCustomCharacterPart>>* CharacterPartsPtr = CharacterPartsProperty->ContainerPtrToValuePtr<TArray<TSoftObjectPtr<UCustomCharacterPart>>>(HeroSpecialization);
			if (CharacterPartsPtr)
			{
				CharacterPartsPtr->Empty();
				for (UCustomCharacterPart* Part : CharacterParts)
				{
					CharacterPartsPtr->Add(TSoftObjectPtr<UCustomCharacterPart>(Part));
				}
			}
		}
		
		// Set AlterationType to "Hero Specialization Tier One Slot"
		FProperty* AlterationSlotsProperty = HeroSpecialization->GetClass()->FindPropertyByName(FName("AlterationSlots"));
		if (AlterationSlotsProperty)
		{
			TArray<FName>* AlterationSlotsPtr = AlterationSlotsProperty->ContainerPtrToValuePtr<TArray<FName>>(HeroSpecialization);
			if (AlterationSlotsPtr)
			{
				AlterationSlotsPtr->Empty();
				AlterationSlotsPtr->Add(FName("Hero Specialization Tier One Slot"));
			}
		}
		
		FAssetRegistryModule::AssetCreated(HeroSpecialization);
		SpecializationPackage->MarkPackageDirty();
		CreatedAssets.Add(HeroSpecialization);
	}
	
	// Create Hero Type at /Game/Surge/Characters/{Gender}/{ID}/Hero_{ID}
	FString HeroTypeName = FString::Printf(TEXT("Hero_%s"), *SkinCodeName);
	FString HeroTypePackageName = SurgePath / HeroTypeName;
	
	UPackage* HeroTypePackage = CreatePackage(nullptr, *HeroTypePackageName);
	UFortHeroType* HeroType = nullptr;
	
	// Duplicate from base if available
	if (!BaseHeroTypePath.IsEmpty())
	{
		UFortHeroType* BaseHeroType = LoadObject<UFortHeroType>(nullptr, *BaseHeroTypePath);
		if (BaseHeroType)
		{
			HeroType = DuplicateObject<UFortHeroType>(BaseHeroType, HeroTypePackage, *HeroTypeName);
			if (HeroType)
			{
				HeroType->SetFlags(RF_Public | RF_Standalone);
			}
		}
	}
	
	if (!HeroType)
	{
		HeroType = NewObject<UFortHeroType>(HeroTypePackage, *HeroTypeName, RF_Public | RF_Standalone);
	}
	
	if (HeroType)
	{
		// Use reflection to set protected Specializations array
		FProperty* SpecializationsProperty = HeroType->GetClass()->FindPropertyByName(FName("Specializations"));
		if (SpecializationsProperty)
		{
			TArray<TSoftObjectPtr<UFortHeroSpecialization>>* SpecializationsPtr = SpecializationsProperty->ContainerPtrToValuePtr<TArray<TSoftObjectPtr<UFortHeroSpecialization>>>(HeroType);
			if (SpecializationsPtr)
			{
				SpecializationsPtr->Empty();
				SpecializationsPtr->Add(TSoftObjectPtr<UFortHeroSpecialization>(HeroSpecialization));
			}
		}
		
		// Set icon on HeroType if provided
		if (!IconPath.IsEmpty())
		{
			UTexture2D* Icon = LoadObject<UTexture2D>(nullptr, *IconPath);
			if (Icon)
			{
				FProperty* SmallPreviewImageProperty = HeroType->GetClass()->FindPropertyByName(FName("SmallPreviewImage"));
				if (SmallPreviewImageProperty)
				{
					TSoftObjectPtr<UTexture2D>* SmallPreviewImagePtr = SmallPreviewImageProperty->ContainerPtrToValuePtr<TSoftObjectPtr<UTexture2D>>(HeroType);
					if (SmallPreviewImagePtr)
					{
						*SmallPreviewImagePtr = TSoftObjectPtr<UTexture2D>(Icon);
					}
				}
			}
		}
		
		FAssetRegistryModule::AssetCreated(HeroType);
		HeroTypePackage->MarkPackageDirty();
		CreatedAssets.Add(HeroType);
	}
	
	// Create Athena Character Item Definition at /Game/Athena/Items/Cosmetics/Characters/Character_{ID}_Surge
	FString CharacterDefName = FString::Printf(TEXT("Character_%s_Asher"), *SkinCodeName);
	FString CharacterDefPackageName = CharacterDefPath / CharacterDefName;
	
	UPackage* CharacterDefPackage = CreatePackage(nullptr, *CharacterDefPackageName);
	UAthenaCharacterItemDefinition* CharacterDef = nullptr;
	
	// Duplicate from base if available
	if (!BaseCIDPath.IsEmpty())
	{
		UAthenaCharacterItemDefinition* BaseCID = LoadObject<UAthenaCharacterItemDefinition>(nullptr, *BaseCIDPath);
		if (BaseCID)
		{
			CharacterDef = DuplicateObject<UAthenaCharacterItemDefinition>(BaseCID, CharacterDefPackage, *CharacterDefName);
			if (CharacterDef)
			{
				CharacterDef->SetFlags(RF_Public | RF_Standalone);
			}
		}
	}
	
	if (!CharacterDef)
	{
		CharacterDef = NewObject<UAthenaCharacterItemDefinition>(CharacterDefPackage, *CharacterDefName, RF_Public | RF_Standalone);
	}
	
	if (CharacterDef)
	{
		CharacterDef->HeroDefinition = HeroType;
		
		// Set display name and description using reflection
		FProperty* DisplayNameProperty = CharacterDef->GetClass()->FindPropertyByName(FName("DisplayName"));
		if (DisplayNameProperty)
		{
			FText* DisplayNamePtr = DisplayNameProperty->ContainerPtrToValuePtr<FText>(CharacterDef);
			if (DisplayNamePtr)
			{
				*DisplayNamePtr = FText::FromString(DisplayName);
			}
		}
		
		FProperty* DescriptionProperty = CharacterDef->GetClass()->FindPropertyByName(FName("Description"));
		if (DescriptionProperty)
		{
			FText* DescriptionPtr = DescriptionProperty->ContainerPtrToValuePtr<FText>(CharacterDef);
			if (DescriptionPtr)
			{
				*DescriptionPtr = FText::FromString(Description);
			}
		}
		
		// Set icons if provided using reflection
		if (!IconLPath.IsEmpty())
		{
			UTexture2D* IconL = LoadObject<UTexture2D>(nullptr, *IconLPath);
			if (IconL)
			{
				FProperty* LargePreviewImageProperty = CharacterDef->GetClass()->FindPropertyByName(FName("LargePreviewImage"));
				if (LargePreviewImageProperty)
				{
					TSoftObjectPtr<UTexture2D>* LargePreviewImagePtr = LargePreviewImageProperty->ContainerPtrToValuePtr<TSoftObjectPtr<UTexture2D>>(CharacterDef);
					if (LargePreviewImagePtr)
					{
						*LargePreviewImagePtr = TSoftObjectPtr<UTexture2D>(IconL);
					}
				}
			}
		}
		if (!IconPath.IsEmpty())
		{
			UTexture2D* Icon = LoadObject<UTexture2D>(nullptr, *IconPath);
			if (Icon)
			{
				FProperty* SmallPreviewImageProperty = CharacterDef->GetClass()->FindPropertyByName(FName("SmallPreviewImage"));
				if (SmallPreviewImageProperty)
				{
					TSoftObjectPtr<UTexture2D>* SmallPreviewImagePtr = SmallPreviewImageProperty->ContainerPtrToValuePtr<TSoftObjectPtr<UTexture2D>>(CharacterDef);
					if (SmallPreviewImagePtr)
					{
						*SmallPreviewImagePtr = TSoftObjectPtr<UTexture2D>(Icon);
					}
				}
			}
		}
		
		// Set FrontEndItemMontage if provided
		if (!FrontEndItemMontagePath.IsEmpty())
		{
			UAnimMontage* Montage = LoadObject<UAnimMontage>(nullptr, *FrontEndItemMontagePath);
			if (Montage)
			{
				FProperty* FrontEndAnimMontageProperty = CharacterDef->GetClass()->FindPropertyByName(FName("FrontEndAnimMontageIdleOverride"));
				if (FrontEndAnimMontageProperty)
				{
					TSoftObjectPtr<UAnimMontage>* FrontEndAnimMontagePtr = FrontEndAnimMontageProperty->ContainerPtrToValuePtr<TSoftObjectPtr<UAnimMontage>>(CharacterDef);
					if (FrontEndAnimMontagePtr)
					{
						*FrontEndAnimMontagePtr = TSoftObjectPtr<UAnimMontage>(Montage);
					}
				}
			}
		}
		
		FAssetRegistryModule::AssetCreated(CharacterDef);
		CharacterDefPackage->MarkPackageDirty();
		CreatedAssets.Add(CharacterDef);
	}
	
	// Save all packages
	TArray<UPackage*> PackagesToSave;
	for (UObject* Asset : CreatedAssets)
	{
		PackagesToSave.AddUnique(Asset->GetOutermost());
	}
	
	FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, false, false);
	
	// Sync content browser to the main character asset
	if (CharacterDef)
	{
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
		TArray<UObject*> SyncAssets;
		SyncAssets.Add(CharacterDef);
		ContentBrowserModule.Get().SyncBrowserToAssets(SyncAssets);
	}
	
	FMessageDialog::Open(EAppMsgType::Ok, 
		FText::Format(LOCTEXT("CharacterCreated", "Character data assets created successfully!\n\nCreated {0} assets at:\n{1}"), 
			FText::AsNumber(CreatedAssets.Num()), 
			FText::FromString(SurgePath)));
}

#undef LOCTEXT_NAMESPACE

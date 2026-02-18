#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "EFortCustomGender.h"
#include "EFortCustomBodyType.h"

class SCharacterCreationWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCharacterCreationWindow) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
	TSharedPtr<class SWindow> ParentWindow;

private:
	// Character settings
	FString SkinCodeName;
	FString DisplayName;
	FString Description;
	EFortCustomGender Gender;
	TEnumAsByte<EFortCustomBodyType::Type> BodyType;
	FString NotifyType;
	
	// Asset Paths (as strings)
	FString SkelMeshBodyPath;
	FString SkelMeshHeadPath;
	FString SkelMeshFaceAccPath;
	FString IconLPath;
	FString IconPath;
	FString ABPBodyPath;
	FString ABPHeadPath;
	FString ABPFaceAccPath;
	FString BaseHeroTypePath;
	FString BaseHSPath;
	FString BaseCPPath;
	FString BaseCIDPath;
	FString FrontEndItemMontagePath;

	// UI Callbacks
	FReply OnCreateCharacterClicked();
	FReply OnCancelClicked();
	
	void OnSkinCodeNameChanged(const FText& NewText);
	void OnDisplayNameChanged(const FText& NewText);
	void OnDescriptionChanged(const FText& NewText);
	void OnGenderChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo);
	void OnBodyTypeChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo);
	void OnNotifyTypeChanged(const FText& NewText);
	
	void LoadSettings();
	void SaveSettings();
	
	void BrowseForAsset(FString& OutPath, UClass* AssetClass);
	
	void CreateCharacterAssets();
	
	// Gender options
	TArray<TSharedPtr<FString>> GenderOptions;
	TSharedPtr<FString> SelectedGender;
	
	// Body Type options
	TArray<TSharedPtr<FString>> BodyTypeOptions;
	TSharedPtr<FString> SelectedBodyType;
};

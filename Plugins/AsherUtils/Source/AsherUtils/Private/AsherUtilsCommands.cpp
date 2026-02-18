#include "AsherUtilsCommands.h"

#define LOCTEXT_NAMESPACE "FAsherUtilsModule"

void FAsherUtilsCommands::RegisterCommands()
{
	UI_COMMAND(
		CreateCharacterDataAsset,
		"Asher Utils",
		"easy tool for making cids. made by asher",
		EUserInterfaceActionType::Button,
		FInputChord(EKeys::Invalid)
	);
}

#undef LOCTEXT_NAMESPACE

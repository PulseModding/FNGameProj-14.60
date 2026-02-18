#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "AsherUtilsStyle.h"

class FAsherUtilsCommands : public TCommands<FAsherUtilsCommands>
{
public:
	FAsherUtilsCommands()
		: TCommands<FAsherUtilsCommands>(TEXT("AsherUtils"), NSLOCTEXT("Contexts", "AsherUtils", "AsherUtils Plugin"), NAME_None, FAsherUtilsStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> CreateCharacterDataAsset;
};

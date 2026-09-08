#pragma once

#include "CoreMinimal.h"
#include "ColorEnum.generated.h"

UENUM(BlueprintType)
enum class ERainbowColor : uint8 {
    None,
    Red,
    Orange,
    Yellow,
	Green,
	Blue,
	Violet,
	White,
	Black
};
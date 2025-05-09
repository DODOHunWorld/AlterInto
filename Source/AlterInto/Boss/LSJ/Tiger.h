#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Tiger.generated.h"

UCLASS()
class ALTERINTO_API ATiger : public ACharacter
{
	GENERATED_BODY()

public:
	ATiger();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

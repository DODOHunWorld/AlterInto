#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TreeBoss.generated.h"

UCLASS()
class ALTERINTO_API ATreeBoss : public ACharacter
{
	GENERATED_BODY()

public:
	ATreeBoss();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

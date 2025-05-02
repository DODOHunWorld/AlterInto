#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerBase.generated.h"

UCLASS()
class ALTERINTO_API APlayerBase : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Getters & Setters
	bool GetCanDash();

protected:
	/* IMC */
    UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* imcPlayer;


	/* IA - Movement */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* iaMove;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* iaTurn;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* iaJump;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* iaDash;


	/* Movement - Variables */
	// Speed
	UPROPERTY(EditDefaultsOnly, Category = "Move|Default")
	float playerSpeed = 600;

	// Turn
	UPROPERTY(EditDefaultsOnly, Category = "Move|Default")
	float minPitch = -78.f;

	UPROPERTY(EditDefaultsOnly, Category = "Move|Default")
	float maxPitch = 50.f;

	// Jump
	bool bIsFalling = false;

	int32 playerJumpCurrentCount;

	// Dash
	bool bCanDash = false;

	FVector prevPos;
	FVector dashStartPos;
	FVector dashEndPos;

    UPROPERTY(EditDefaultsOnly, Category = "Move|Dash")
	float dashDistance = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "Move|Dash")
	float dashCurrentTime = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Move|Dash")
	float dashDurationTime = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Move|Dash")
	float dashCoolDownTime = 0.5f;


	/* Movement - Functions */
	void DoMove(const struct FInputActionValue& InValue);

	void DoTurn(const struct FInputActionValue& InValue);

	void DoJump(const struct FInputActionValue& InValue);

	/*virtual */void StartDash(const struct FInputActionValue& InValue);
	void DoDash(float InDeltaTime);
	void ResetDash(float InDeltaTime);
};

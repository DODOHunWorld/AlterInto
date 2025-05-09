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

	/* Getters */ 
	bool GetCanDash();

    /* Setters */

protected:
    /* Camera - Spring Arm */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera|SpringArm")
    class USpringArmComponent* springArmComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera|SpringArm")
    float springArmLengthDefault = 400.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera|SpringArm")
    FVector springArmLocationDefault = FVector(-80.f, 0.f, 140.f);


	/* Camera - Camera */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera|Camera")
    class UCameraComponent* cameraComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera|Camera")
    FRotator cameraRotationDefault = FRotator(-2.f, 0.f, 0.f);


	/* Camera - Scene Capture : split 화면 구성 시 필요 / 일단 변수만 추가해둠 */ 
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera|SplitView")
    class USceneCaptureComponent* sceneCaptureComp;


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


	/* Movement Variables - Speed */
	UPROPERTY(EditDefaultsOnly, Category = "Move|Default")
	float playerSpeed = 600;

	/* Movement Variables - Turn */
	UPROPERTY(EditAnywhere, Category = "Move|Default")
	float minPitch = -60.f;

	UPROPERTY(EditAnywhere, Category = "Move|Default")
	float maxPitch = 20.f;

	/* Movement Variables - Jump */
	bool bIsFalling = false;

	int32 playerJumpCurrentCount;

	/* Movement Variables - Dash */
	bool bIsDashing = false;
    bool bCanResetDash = false;

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


	/* Statement */
    UPROPERTY(EditDefaultsOnly, Category = "State")
	int32 HP = 10;
	
    UPROPERTY(EditDefaultsOnly, Category = "State")
    bool bIsDamaged = false;

    UPROPERTY(EditDefaultsOnly, Category = "State")
	bool bIsDead = false;


	/* Movement - Functions */
	void DoMove(const struct FInputActionValue& InValue);

	void DoTurn(const struct FInputActionValue& InValue);

	void DoJump(const struct FInputActionValue& InValue);

	/*virtual */void StartDash(const struct FInputActionValue& InValue);
	void DoDash(float InDeltaTime);
	void ResetDash(float InDeltaTime);
};

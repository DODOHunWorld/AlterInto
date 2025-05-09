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
	bool GetIsDodging();

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


	/* IMC */
    UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* imcPlayer;


	/* IA - Movement */
	UPROPERTY(EditDefaultsOnly, Category = "Input|Movement")
	class UInputAction* iaMove;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Movement")
	class UInputAction* iaTurn;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Movement")
	class UInputAction* iaJump;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Movement")
	class UInputAction* iaDodge;


	/* IA - Combat */
	UPROPERTY(EditDefaultsOnly, Category = "Input|Combat")
	class UInputAction* iaAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Combat")
	class UInputAction* iaSkillE;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Combat")
	class UInputAction* iaSkillQ;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Combat")
	class UInputAction* iaGuard;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Combat")
	class UInputAction* iaLockOn;


	/* IA - Etc */
	UPROPERTY(EditDefaultsOnly, Category = "Input|Etc")
	class UInputAction* iaPlayer1;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Etc")
	class UInputAction* iaPlayer2;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Etc")
	class UInputAction* iaRidePet;


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

	/* Movement Variables - Dodge */
	bool bIsDodging = false;
    bool bCanResetDodge = false;

	FVector prevPos;
	FVector dodgeStartPos;
	FVector dodgeEndPos;

    UPROPERTY(EditDefaultsOnly, Category = "Move|Dash")
	float dodgeDistance = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "Move|Dash")
	float dodgeCurrentTime = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Move|Dash")
	float dodgeDurationTime = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Move|Dash")
	float dodgeCoolDownTime = 0.5f;


	/* Statement */
    UPROPERTY(EditDefaultsOnly, Category = "State")
	int32 HP = 10;
	
    UPROPERTY(EditDefaultsOnly, Category = "State")
    bool bIsDamaged = false;

    UPROPERTY(EditDefaultsOnly, Category = "State")
	bool bIsDead = false;


	/* Lock On */
	bool bIsLockedOn = false;


	/* Change Player*/
	bool bIsPlayer1 = false;

	/* Pet */
	bool bIsRidingPet = false;


	/* Movement */
	void DoMove(const struct FInputActionValue& InValue);

	void DoTurn(const struct FInputActionValue& InValue);

	void DoJump(const struct FInputActionValue& InValue);

	/*virtual */void StartDodge(const struct FInputActionValue& InValue);
	void DoDodge(float InDeltaTime);
	void ResetDodge(float InDeltaTime);


	/* Combat */
	virtual void Attack(const struct FInputActionValue& InValue);

	virtual void SkillE(const struct FInputActionValue& InValue);;

	virtual void SkillQ(const struct FInputActionValue& InValue);

	virtual void Guard(const struct FInputActionValue& InValue);

	void LockOnOff(const struct FInputActionValue& InValue);


	/* Etc */
	void ChangeCharacter1(const struct FInputActionValue& InValue);

	void ChangeCharacter2(const struct FInputActionValue& InValue);

	void ChangeCharacter();

	void RidePetOnOff(const struct FInputActionValue& InValue);

};

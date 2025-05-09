#include "Player/DYL/PlayerBase.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APlayerBase::APlayerBase()
{
	PrimaryActorTick.bCanEverTick = true;

	/* Camera - Spring Arm */
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->TargetArmLength = springArmLengthDefault;
    springArmComp->SetRelativeLocation(springArmLocationDefault);
    springArmComp->bUsePawnControlRotation = true;


	/* Camera - Camera */
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    cameraComp->SetupAttachment(springArmComp);
	cameraComp->SetRelativeRotation(cameraRotationDefault);
	cameraComp->bUsePawnControlRotation = false;
	

	/* IMC */
	ConstructorHelpers::FObjectFinder<UInputMappingContext> tmpIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Player/DYL/Inputs/IMC_Player.IMC_Player'"));
	if(tmpIMC.Succeeded()) imcPlayer = tmpIMC.Object;


	/* IA - Movement */
	ConstructorHelpers::FObjectFinder<UInputAction> tmpMove(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/DYL/Inputs/IA_Move.IA_Move'"));
	if (tmpMove.Succeeded()) iaMove = tmpMove.Object;

	ConstructorHelpers::FObjectFinder<UInputAction> tmpTurn(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/DYL/Inputs/IA_Turn.IA_Turn'"));
	if (tmpTurn.Succeeded()) iaTurn = tmpTurn.Object;

	ConstructorHelpers::FObjectFinder<UInputAction> tmpJump(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/DYL/Inputs/IA_Jump.IA_Jump'"));
	if (tmpJump.Succeeded()) iaJump = tmpJump.Object;

	ConstructorHelpers::FObjectFinder<UInputAction> tmpDash(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/DYL/Inputs/IA_Dodge.IA_Dodge'"));
	if (tmpDash.Succeeded()) iaDodge = tmpDash.Object;


	/* IA - Combat */
	ConstructorHelpers::FObjectFinder<UInputAction> tmpAttack(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/DYL/Inputs/IA_Attack.IA_Attack'"));
	if (tmpAttack.Succeeded()) iaAttack = tmpAttack.Object;

	ConstructorHelpers::FObjectFinder<UInputAction> tmpSkillE(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/DYL/Inputs/IA_SkillE.IA_SkillE'"));
	if (tmpSkillE.Succeeded()) iaSkillE = tmpSkillE.Object;

	ConstructorHelpers::FObjectFinder<UInputAction> tmpSkillQ(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/DYL/Inputs/IA_SkillQ.IA_SkillQ'"));
	if (tmpSkillQ.Succeeded()) iaSkillQ = tmpSkillQ.Object;

	ConstructorHelpers::FObjectFinder<UInputAction> tmpGuard(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/DYL/Inputs/IA_Guard.IA_Guard'"));
	if (tmpGuard.Succeeded()) iaGuard = tmpGuard.Object;

	ConstructorHelpers::FObjectFinder<UInputAction> tmpLock(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/DYL/Inputs/IA_LockOn.IA_LockOn'"));
	if (tmpLock.Succeeded()) iaLockOn = tmpLock.Object;


	/* IA - Etc */
	ConstructorHelpers::FObjectFinder<UInputAction> tmp1(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/DYL/Inputs/IA_Player1.IA_Player1'"));
	if (tmp1.Succeeded()) iaPlayer1 = tmp1.Object;

	ConstructorHelpers::FObjectFinder<UInputAction> tmp2(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/DYL/Inputs/IA_Player2.IA_Player2'"));
	if (tmp2.Succeeded()) iaPlayer2 = tmp2.Object;

	ConstructorHelpers::FObjectFinder<UInputAction> tmpPet(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/DYL/Inputs/IA_RidePet.IA_RidePet'"));
	if (tmpPet.Succeeded()) iaRidePet = tmpPet.Object;


    /* Movement - Rotation */
	bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;


    GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	/* Movement - Speed */
    GetCharacterMovement()->MaxWalkSpeed = playerSpeed;

	/* Movement - Jump */
	JumpMaxCount = 2;
}

void APlayerBase::BeginPlay()
{
	Super::BeginPlay();
	
	bIsLockedOn = false;
	bIsRidingPet = false;
}

void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* Dodge */
    if (bIsDodging) DoDodge(DeltaTime);
    if (bCanResetDodge) ResetDodge(DeltaTime);
}

void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/* IMC */
	APlayerController* pc = Cast<APlayerController>
	(GetController());
	if (pc)
	{
		auto localPlayer = pc->GetLocalPlayer();
		auto ss = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(localPlayer);
		if(ss) ss->AddMappingContext(imcPlayer, 0);
	}

	/* IA */
	auto input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (input)
	{
		// Movement
		input->BindAction(iaMove, ETriggerEvent::Triggered, this, &APlayerBase::DoMove);
		input->BindAction(iaTurn, ETriggerEvent::Triggered, this, &APlayerBase::DoTurn);
		input->BindAction(iaJump, ETriggerEvent::Started, this, &APlayerBase::DoJump);
		input->BindAction(iaDodge, ETriggerEvent::Started, this, &APlayerBase::StartDodge);

		// Combat
		input->BindAction(iaAttack, ETriggerEvent::Started, this, &APlayerBase::Attack);
		input->BindAction(iaSkillE, ETriggerEvent::Started, this, &APlayerBase::SkillE);
		input->BindAction(iaSkillQ, ETriggerEvent::Started, this, &APlayerBase::SkillQ);
		input->BindAction(iaGuard, ETriggerEvent::Started, this, &APlayerBase::Guard);
		input->BindAction(iaLockOn, ETriggerEvent::Started, this, &APlayerBase::LockOnOff);

		// Etc
		input->BindAction(iaPlayer1, ETriggerEvent::Started, this, &APlayerBase::ChangeCharacter1);
		input->BindAction(iaPlayer2, ETriggerEvent::Started, this, &APlayerBase::ChangeCharacter2);
		input->BindAction(iaRidePet, ETriggerEvent::Started, this, &APlayerBase::RidePetOnOff);
	}
}

bool APlayerBase::GetIsDodging()
{
	return bIsDodging;
}

void APlayerBase::DoMove(const struct FInputActionValue& InValue)
{
    if (bIsDodging || bIsDead) return;

    FVector2d move = InValue.Get<FVector2d>();
    AddMovementInput(cameraComp->GetForwardVector() * move.X + cameraComp->GetRightVector() * move.Y);
}

void APlayerBase::DoTurn(const struct FInputActionValue& InValue)
{
    FVector2d turn = InValue.Get<FVector2d>();
    FRotator rot = GetController()->GetControlRotation();

	rot.Pitch = FMath::Clamp(rot.Pitch + turn.Y, minPitch, maxPitch);
    rot.Yaw += turn.X;

	GetController()->SetControlRotation(rot);
}

void APlayerBase::DoJump(const struct FInputActionValue& InValue)
{
    if (bIsDead) return;

	Jump();
    bIsFalling = GetCharacterMovement()->IsFalling();
	playerJumpCurrentCount = JumpCurrentCount;
}

void APlayerBase::StartDodge(const struct FInputActionValue& InValue)
{
	//UE_LOG(LogTemp, Warning, TEXT(">>> Dodge"));
    if (bIsDead || bIsDodging || bCanResetDodge) return;

    dodgeStartPos = GetActorLocation();
    dodgeEndPos = GetActorLocation() + GetActorForwardVector() * dodgeDistance;
    bIsDodging = true;
    bCanResetDodge = false;
}

void APlayerBase::DoDodge(float InDeltaTime)
{
    dodgeCurrentTime += InDeltaTime;
    if (dodgeCurrentTime >= dodgeDurationTime)
    {
        bIsDodging = false;
        bCanResetDodge = true;
        dodgeCurrentTime = 0.f;
        SetActorLocation(dodgeEndPos);
    }
    else
    {
        SetActorLocation( FMath::Lerp(dodgeStartPos, dodgeEndPos, dodgeCurrentTime / dodgeDurationTime) );
    }
}

void APlayerBase::ResetDodge(float InDeltaTime)
{
    if (bCanResetDodge)
    {
        dodgeCurrentTime += InDeltaTime;
        if (dodgeCurrentTime >= dodgeCoolDownTime)
        {
            bCanResetDodge = false;
            dodgeCurrentTime = 0.f;
        }
    }
}

void APlayerBase::Attack(const struct FInputActionValue& InValue)
{
	UE_LOG(LogTemp, Warning, TEXT(">>> Attack"));
}

void APlayerBase::SkillE(const struct FInputActionValue& InValue)
{
	UE_LOG(LogTemp, Warning, TEXT(">>> SkillE"));
}

void APlayerBase::SkillQ(const struct FInputActionValue& InValue)
{
	UE_LOG(LogTemp, Warning, TEXT(">>> SkillQ"));
}

void APlayerBase::Guard(const struct FInputActionValue& InValue)
{
	UE_LOG(LogTemp, Warning, TEXT(">>> Guard"));
}

void APlayerBase::LockOnOff(const struct FInputActionValue& InValue)
{
	bIsLockedOn = !bIsLockedOn;
	UE_LOG(LogTemp, Warning, TEXT(">>> LockOnOff - %d"), bIsLockedOn);
}


void APlayerBase::ChangeCharacter1(const struct FInputActionValue& InValue)
{
	if(bIsPlayer1) return;

	UE_LOG(LogTemp, Warning, TEXT(">>> Character 1"));
	bIsPlayer1 = true;
	ChangeCharacter();
}

void APlayerBase::ChangeCharacter2(const struct FInputActionValue& InValue)
{
	if (!bIsPlayer1) return;

	UE_LOG(LogTemp, Warning, TEXT(">>> Character 2"));
	bIsPlayer1 = false;
	ChangeCharacter();
}

void APlayerBase::ChangeCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT(">>> ChangeCharacter"));
}


void APlayerBase::RidePetOnOff(const struct FInputActionValue& InValue)
{
	bIsRidingPet = !bIsRidingPet;
	UE_LOG(LogTemp, Warning, TEXT(">>> RidePetOnOff - %d"), bIsRidingPet);
}

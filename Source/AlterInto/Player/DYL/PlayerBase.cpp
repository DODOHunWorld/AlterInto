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


    /* Camera - Scene Capture */
	

	/* IMC */
	ConstructorHelpers::FObjectFinder<UInputMappingContext> tmpIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Player/DYL/Inputs/IMC_Player.IMC_Player'"));
	if(tmpIMC.Succeeded()) imcPlayer = tmpIMC.Object;


	/* IA - Movement */
	//ConstructorHelpers::FObjectFinder<UInputAction> tmp(TEXT(""));
	//if (tmp.Succeeded()) ia = tmp.Object;
	ConstructorHelpers::FObjectFinder<UInputAction> tmpMove(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/DYL/Inputs/IA_Move.IA_Move'"));
	if (tmpMove.Succeeded()) iaMove = tmpMove.Object;

	ConstructorHelpers::FObjectFinder<UInputAction> tmpTurn(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/DYL/Inputs/IA_Turn.IA_Turn'"));
	if (tmpTurn.Succeeded()) iaTurn = tmpTurn.Object;

	ConstructorHelpers::FObjectFinder<UInputAction> tmpJump(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/DYL/Inputs/IA_Jump.IA_Jump'"));
	if (tmpJump.Succeeded()) iaJump = tmpJump.Object;

	ConstructorHelpers::FObjectFinder<UInputAction> tmpDash(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/DYL/Inputs/IA_Dash.IA_Dash'"));
	if (tmpDash.Succeeded()) iaDash = tmpDash.Object;


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
	
}

void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* Dash */
    if (bIsDashing) DoDash(DeltaTime);
    if (bCanResetDash) ResetDash(DeltaTime);
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
		input->BindAction(iaDash, ETriggerEvent::Started, this, &APlayerBase::StartDash);
	}
}

bool APlayerBase::GetCanDash()
{
	return bIsDashing;
}

void APlayerBase::DoMove(const struct FInputActionValue& InValue)
{
    if (bIsDashing || bIsDead) return;

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

void APlayerBase::StartDash(const struct FInputActionValue& InValue)
{
	//UE_LOG(LogTemp, Warning, TEXT(">>> dash"));
    if (bIsDead || bIsDashing || bCanResetDash) return;

    dashStartPos = GetActorLocation();
    dashEndPos = GetActorLocation() + GetActorForwardVector() * dashDistance;
    bIsDashing = true;
    bCanResetDash = false;
}

void APlayerBase::DoDash(float InDeltaTime)
{
    dashCurrentTime += InDeltaTime;
    if (dashCurrentTime >= dashDurationTime)
    {
        bIsDashing = false;
        bCanResetDash = true;
        dashCurrentTime = 0.f;
        SetActorLocation(dashEndPos);
    }
    else
    {
        SetActorLocation( FMath::Lerp(dashStartPos, dashEndPos, dashCurrentTime / dashDurationTime) );
    }
}

void APlayerBase::ResetDash(float InDeltaTime)
{
    if (bCanResetDash)
    {
        dashCurrentTime += InDeltaTime;
        if (dashCurrentTime >= dashCoolDownTime)
        {
            bCanResetDash = false;
            dashCurrentTime = 0.f;
        }
    }
}

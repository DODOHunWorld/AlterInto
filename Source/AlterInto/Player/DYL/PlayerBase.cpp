#include "Player/DYL/PlayerBase.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

APlayerBase::APlayerBase()
{
	PrimaryActorTick.bCanEverTick = true;

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
}

void APlayerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	return bCanDash;
}

void APlayerBase::DoMove(const struct FInputActionValue& InValue)
{
	UE_LOG(LogTemp, Warning, TEXT(">>> move"));
}

void APlayerBase::DoTurn(const struct FInputActionValue& InValue)
{
	UE_LOG(LogTemp, Warning, TEXT(">>> turn"));
}

void APlayerBase::DoJump(const struct FInputActionValue& InValue)
{
	UE_LOG(LogTemp, Warning, TEXT(">>> jump"));

}

void APlayerBase::StartDash(const struct FInputActionValue& InValue)
{
	UE_LOG(LogTemp, Warning, TEXT(">>> dash"));
}

void APlayerBase::DoDash(float InDeltaTime)
{

}

void APlayerBase::ResetDash(float InDeltaTime)
{

}


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PeackPlayerController.generated.h"

class UCharacterWidget;
class UPlayerStateWidget;
class APeackPlayerState;
class UWarmupWidget;

UCLASS()
class PEACK_API APeackPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void ReceivedPlayer() override;

	void CreateWidget_Character();
	void CreateWidget_PlayerState();
	void CreateWidget_Warmup();
	void ReadyPlayerState(APeackPlayerState* GivenPlayerState);

	void UpdateBar_Health(float Health, float MaxHealth);
	void UpdateText_Score(float GivenScore);
	void UpdateText_Death(float GivenDeath);
	void UpdateText_Countdown(int TimeLeft);

	void GameModeChangedMatchState(const FName NewMatchState);

	UFUNCTION(Client, Reliable)
	void Client_GameModeChangedMatchState(const FName NewMatchState);

	void GameModeSendInformations(const FName GivenMatchState);

	UFUNCTION(Client, Reliable)
	void Client_GameModeSendInformations(const FName GivenMatchState);

	void HandleMatchState(const FName GivenMatchState);

private:
	double GetWorldTime() const;
	double GetWorldTime_Server() const;

	UFUNCTION(Server, Reliable)
	void Server_RequestServerTime(double RequestTimeFromClient);

	UFUNCTION(Client, Reliable)
	void Client_ReportServerTimeToClient(double RequestTimeFromClient, double ReceverTimeFromServer);

private:
	FName CurrentMatchState;

	UPROPERTY()
	TObjectPtr<APeackPlayerState> PeackPlayerState;

	/*
	* Countdown
	*/
	double TotalTime_Match = 20.0; 

	double LastCountdown = 0.0;

	double Detal_Server_Client = 0.0;

	/*
	* Character Widget
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> WidgetClass_Character;

	UPROPERTY()
	TObjectPtr<UCharacterWidget> Widget_Character;

	/*
	* PlayerState Widget
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> WidgetClass_PlayerState;

	UPROPERTY()
	TObjectPtr<UPlayerStateWidget> Widget_PlayerState;

	/*
	* Warmup Widget
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> WidgetClass_Warmup;

	UPROPERTY()
	TObjectPtr<UWarmupWidget> Widget_Warmup;
	
};

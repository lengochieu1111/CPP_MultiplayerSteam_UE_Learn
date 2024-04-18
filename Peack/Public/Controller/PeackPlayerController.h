
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PeackPlayerController.generated.h"

class UCharacterWidget;
class APeackPlayerState;
class UWarmupWidget;
class UPlayerStateWidget;
class UShowResultWidget;

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
	void CreateWidget_ShowResult();

	void ReadyPlayerState(APeackPlayerState* GivenPlayerState);

	void UpdateBar_Health(float Health, float MaxHealth);
	void UpdateText_Score(float GivenScore);
	void UpdateText_Death(float GivenDeath);

	void GameModeChangedMatchState(const FName NewMatchState);

	UFUNCTION(Client, Reliable)
	void Client_GameModeChangedMatchState(const FName NewMatchState);

	void GameModeSendInformations(
		const double SLT,
		const FName GivenMatchState,
		const double TotalTimeWarmup,
		const double TotalTimeMatch,
		const double TotalTimeShowResult);

	UFUNCTION(Client, Reliable)
	void Client_GameModeSendInformations(
		const double SLT,
		const FName GivenMatchState,
		const double TotalTimeWarmup,
		const double TotalTimeMatch,
		const double TotalTimeShowResult);

	void HandleMatchState(const FName GivenMatchState);

private:
	double GetWorldTime() const;
	double GetWorldTime_Server() const;

	UFUNCTION(Server, Reliable)
	void Server_RequestServerTime(double RequestTimeFromClient);

	UFUNCTION(Client, Reliable)
	void Client_ReportServerTimeToClient(double RequestTimeFromClient, double ReceverTimeFromServer);

	void UpdateCountdown();
	void UpdateCountdown_Warmup();
	void UpdateCountdown_InMatch();
	void UpdateCountdown_ShowResult();

private:
	FName CurrentMatchState;

	UPROPERTY()
	TObjectPtr<APeackPlayerState> PeackPlayerState;

	/*
	* Countdown
	*/
	double StartLevelTime = 0.0; 

	double TotalTime_Match = 0.0; 

	double TotalTime_Warmup = 0.0; 

	double TotalTime_ShowResult = 0.0; 


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

	/*
	* Warmup Widget
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> WidgetClass_ShowResult;

	UPROPERTY()
	TObjectPtr<UShowResultWidget> Widget_ShowResult;
	
};

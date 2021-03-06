// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "UObject/ScriptMacros.h"
#include "Interfaces/OnlineStoreInterface.h"
#include "InAppPurchaseRestoreCallbackProxy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInAppPurchaseRestoreResult, EInAppPurchaseState::Type, CompletionStatus, const TArray<FInAppPurchaseRestoreInfo>&, InAppRestorePurchaseInformation);

UCLASS(MinimalAPI)
class UE_DEPRECATED(4.26, "UInAppPurchaseRestoreCallbackProxy is deprecated, please use UInAppPurchaseRestoreCallbackProxy2 instead.") UInAppPurchaseRestoreCallbackProxy : public UObject
{
	GENERATED_UCLASS_BODY()

	// Called when there is a successful In-App Purchase transaction
	UPROPERTY(BlueprintAssignable)
	FInAppPurchaseRestoreResult OnSuccess;

	// Called when there is an unsuccessful In-App Purchase transaction
	UPROPERTY(BlueprintAssignable)
	FInAppPurchaseRestoreResult OnFailure;

	// Kicks off a transaction for the provided product identifier
	UFUNCTION(BlueprintCallable, meta = (DisplayName="Restore In-App Purchases"), Category="Online|InAppPurchase")
	static UInAppPurchaseRestoreCallbackProxy* CreateProxyObjectForInAppPurchaseRestore(const TArray<FInAppPurchaseProductRequest>& ConsumableProductFlags, class APlayerController* PlayerController);

public:

	//~ Begin UObject Interface
	virtual void BeginDestroy() override;
	//~ End UObject Interface

private:

	/** Called by the InAppPurchase system when the transaction has finished */
	void OnInAppPurchaseRestoreComplete_Delayed();
	void OnInAppPurchaseRestoreComplete(EInAppPurchaseState::Type CompletionState);

	/** Unregisters our delegate from the In-App Purchase system */
	void RemoveDelegate();

	/** Triggers the In-App Purchase Restore Transaction for the specifed user */
	void Trigger(const TArray<FInAppPurchaseProductRequest>& ConsumableProductFlags, class APlayerController* PlayerController);

private:

	/** Delegate called when a InAppPurchase has been successfully restored */
	FOnInAppPurchaseRestoreCompleteDelegate InAppPurchaseRestoreCompleteDelegate;

	/** Handle to the registered InAppPurchaseCompleteDelegate */
	FDelegateHandle InAppPurchaseRestoreCompleteDelegateHandle;

	/** The InAppPurchaseRestore read request */
	FOnlineInAppPurchaseRestoreReadPtr ReadObject;

	/** Did we fail immediately? */
	bool bFailedToEvenSubmit;

	/** Pointer to the world, needed to delay the results slightly */
	TWeakObjectPtr<UWorld> WorldPtr;

	/** Did the purchase succeed? */
	EInAppPurchaseState::Type SavedPurchaseState;
	TArray<FInAppPurchaseRestoreInfo> SavedProductInformation;
};

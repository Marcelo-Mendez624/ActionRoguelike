// Fill out your copyright notice in the Description page of Project Settings.


#include "SWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTIme)
{
	Super::NativeTick(MyGeometry, InDeltaTIme);

	if(!IsValid(AttachActor))
	{
		RemoveFromParent();
		return;
	}
	
	FVector2D ScreenPosition;
	if(UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachActor->GetActorLocation(), ScreenPosition))
	{
		const float Scale = UWidgetLayoutLibrary::GetViewportScale(this);

		ScreenPosition /= Scale;

		if(ParentSizeBox)
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
	}
	
	
}

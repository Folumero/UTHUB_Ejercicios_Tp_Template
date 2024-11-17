#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TAttributes.generated.h"


template <typename T>
class Attribute
{
public:

	Attribute(T InValue = T(), const FName& InAttributeName = TEXT("NONE"))
		: Value(InValue), AttributeName(InAttributeName) {}

	T GetValue() const { return Value; }
	void SetValue(T InValue) { Value = InValue; }

	const FName& GetAttributeName() const { return AttributeName; }

private:
	T Value;
	FName AttributeName;
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UTHUB_EJERCICIOSC_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAttributeComponent();


	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void CreateAttribute(const FName& AttributeName, float Value);


	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void GetAttributeValue(const FName& AttributeName, float& OutValue) const;


	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void ModifyAttribute(const FName& AttributeName, float NewValue);

protected:
	virtual void BeginPlay() override;

private:
	TArray<Attribute<float>> FloatAttributes;
};

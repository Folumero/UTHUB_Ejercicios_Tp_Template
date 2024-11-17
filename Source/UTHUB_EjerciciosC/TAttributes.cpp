#include "TAttributes.h"

UAttributeComponent::UAttributeComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UAttributeComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UAttributeComponent::CreateAttribute(const FName& AttributeName, float Value)
{
    for (const auto& Attr : FloatAttributes)
    {
        if (Attr.GetAttributeName() == AttributeName)
        {
            //Para devolverle el valor
            return;
        }
    }

    FloatAttributes.Add(Attribute<float>(Value, AttributeName));
}

void UAttributeComponent::GetAttributeValue(const FName& AttributeName, float& OutValue) const
{
    for (const auto& Attr : FloatAttributes)
    {
        if (Attr.GetAttributeName() == AttributeName)
        {
            OutValue = Attr.GetValue();
            return;
        }
    }
    
}

void UAttributeComponent::ModifyAttribute(const FName& AttributeName, float NewValue)
{
    for (auto& Attr : FloatAttributes)
    {
        if (Attr.GetAttributeName() == AttributeName)
        {
            Attr.SetValue(NewValue);
            return;
        }
    }
}

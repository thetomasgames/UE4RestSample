// Fill out your copyright notice in the Description page of Project Settings.

#include "RestQuery.h"

// Called every frame
void ARestQuery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Sets default values
ARestQuery::ARestQuery(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//When the object is constructed, Get the HTTP module
	Http = &FHttpModule::Get();
}

// Called when the game starts or when spawned
void ARestQuery::BeginPlay()
{
	UE_LOG(LogTemp,Warning,TEXT("Begin play"))
	MyHttpCall();
	Super::BeginPlay();
}

/*Http call*/
void ARestQuery::MyHttpCall()
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ARestQuery::OnResponseReceived);
	//This is the url on which to process the request
	Request->SetURL("http://jsonplaceholder.typicode.com/posts/1");
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

/*Assigned function on successfull http call*/
void ARestQuery::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

	UE_LOG(LogTemp, Warning, TEXT("Response received"))
	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	//Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	//Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		//Get the value of the json object by field name
		int32 recievedInt = JsonObject->GetIntegerField("userId");

		//Output it to the engine
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString::FromInt(recievedInt));
	}
}
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBuild_Book.generated.h"
UENUM(BlueprintType)
enum class EBookCombat : uint8
{
	Open,Close,Max
};

template<typename T>
class FNode 
{
public:
	FNode()
	{

		PreNode = nullptr;
		NextNode = nullptr;
	}
	FNode(T data)
	{
		Data = data;
		PreNode = nullptr;
		NextNode = nullptr;
	}
	FNode* PreNode;
	FNode* NextNode;
	T Data;
};

template<typename T>
class UDLinkNode
{

public:
	void PushNode(T data)
	{
		FNode<T>* newNode = new FNode<T>(data);
		if (size == 0)
			head = newNode;
		else
		{
			FNode<T>* temp = head;
			while (temp->NextNode != nullptr)
				temp = temp->NextNode;
			temp->NextNode = newNode;
			newNode->PreNode = temp;
			newNode->Data = data;
		}
		size++;
	}

	void InsertNodeAfter(FNode<T>* node, T data)
	{
		FNode<T>* new_node = new FNode<T>();
		new_node->Data = data;
		new_node->PreNode = node;
		new_node->NextNode = node->NextNode;
		if (node->NextNode != nullptr)
		{
			node->NextNode->PreNode = new_node;
		}
		node->NextNode = new_node;
		size++;
	}

	// 특정 위치에 노드를 삽입하는 함수

	void InsertNodeAt( int index, T data)
	{
		if (index > size)
		{
			PushNode(data);
			return;
		}
		FNode<T>* temp = head;
		for (int i = 0; i < index; i++)
		{
			temp = temp->NextNode;
		}
		InsertNodeAfter(temp, data);
		size++;
	}
	FNode<T>* GetNode(int index)
	{
		if (index == 0)
			return head;
		
		FNode<T>* temp = head;
		for(int i=0; i < index; i++)
			temp = temp->NextNode;
		return temp;

	}

public:
	FNode<T>* head = nullptr;
	int size = 0;
};

UCLASS()
class PRTFOLIO_12_24_API ACBuild_Book : public AActor
{
	GENERATED_BODY()
	
public:	
	ACBuild_Book();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class USkeletalMeshComponent* mesh;

public:
	UFUNCTION(BlueprintCallable)
		void Open_BuildBook(class USceneComponent* parent);
	UFUNCTION(BlueprintCallable)
		FORCEINLINE bool Open_chk() { return open_Chk; }
	void Page_Update(bool NextPage);

	void Page_In_Book(class ACPageMaster* page);
	void LookPage_Change(bool NextPage);

	FORCEINLINE EBookCombat CurrentCombat() { return combat; }
	FORCEINLINE void Set_Owner(class ACPlayer* OnwerCharacter) { Player = OnwerCharacter; }
	UFUNCTION()
		void Delay_Invisible();
	void OpenBook_Update();
	
	void Select_Build();
	
private:
	UDLinkNode<class ACBuild*>* LinkNode;
	UDLinkNode<class ACPageMaster*>* LinkNode2;

	class ACPageMaster* PageCover;
	bool open_Chk = true;
	int LookPage = 0;


	EBookCombat combat = EBookCombat::Close;
	class ACPlayer* Player; 
};

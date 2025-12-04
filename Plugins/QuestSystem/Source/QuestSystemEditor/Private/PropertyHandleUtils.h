#pragma once


template<typename T>
T* GetHandleValue(TSharedPtr<IPropertyHandle> Handle)
{
	void* CurrentPtr;
	Handle->GetValueData(CurrentPtr);
	return static_cast<T*>(CurrentPtr);
}

template<typename T>
void SetHandleValue(TSharedPtr<IPropertyHandle> Handle, const T& Value)
{
	T* ValuePtr = GetHandleValue<T>(Handle);
	*ValuePtr = Value;
}


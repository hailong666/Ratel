#pragma once

template<typename T>
class CSigleton
{
public:
	CSigleton();
	~CSigleton() ;
	static T& GetInstance(){
		if(value == nullptr){
			value = new T();
		}
		return *value;
	}
private:
	//CSigleton(const CSigleton&) = delete;
	static T* value;

};
template<typename T>
T* CSigleton<T>::value = nullptr;

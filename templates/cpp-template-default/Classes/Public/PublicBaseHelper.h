#pragma once
/*
* to implement a singleton class that does not need constructor
* example:
* class example{
*     SINGLETON_IMPLEMENT(example)
* public:
*     your data;
* };
*/
#define SINGLETON_IMPLEMENT(class_name) \
public : \
	class_name(class_name&) = delete; \
	class_name(class_name&&) = delete; \
	static class_name *getInstance() \
{ \
		static  class_name ins; \
		return &ins; \
} \
private: \
	class_name() {}




/*
* to implement a singleton class that does need a custom constructor
* example:
* class example{
*     SINGLETON_IMPLEMENT_NO_CONSTRUCT(example)
* private:
*     example();
* };
*/
#define SINGLETON_IMPLEMENT_NO_CONSTRUCT(class_name) \
public : \
	class_name(class_name&) = delete; \
	class_name(class_name&&) = delete; \
	static class_name *getInstance() \
{ \
		static  class_name ins; \
		return &ins; \
}
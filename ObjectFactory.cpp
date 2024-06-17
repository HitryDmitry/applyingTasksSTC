#include <map>
#include <string>
#include <iterator>

class IGameObject{
	public:
		virtual ~IGameObject() {}
		virtual void Update() = 0;
		virtual void Render() = 0;
};

class Plane: public IGameObject{
	public:
		Plane() {}
		void Update() {}
		void Render() {}
};

class Boat: public IGameObject{
	public:
		Boat() {}
		void Update() {}
		void Render() {}
};


IGameObject* MakeGameObjectFactory(std::string type){
	if(type=="plane") return new Plane;
	else if(type=="boat") return new Boat;
	return nullptr;
}

int main(){
    IGameObject* myObject = MakeGameObjectFactory("plane");
}
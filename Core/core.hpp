#include <Mod/CppUserModBase.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include <Unreal/AActor.hpp>

namespace ArrND::Core {
    class Core {
        public:
            Core();
            ~Core();
            AActor* GetPlayer();

            bool GetPlayerInstanceFromGame() 
        private:
            AActor* player;
    };
}
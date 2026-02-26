#pragma once
#include<src/client/rendering/rendering.h>

class ClientMain{
    public:
        ClientMain(){}

        ClientRendering& rendering = ClientRendering::GetOnlyInstance();

        void tickClient();
        
};
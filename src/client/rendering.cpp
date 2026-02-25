#include<src/client/rendering.h>

ClientRendering::ClientRendering(){

}

ClientRendering& ClientRendering::GetOnlyInstance(){
    static ClientRendering mainInstance;
    return mainInstance;
}
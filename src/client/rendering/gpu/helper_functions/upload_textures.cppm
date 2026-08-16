module;
#include "SDL3/SDL.h"

export module Pipeline_Textures;
import Test_Shapes;



SDL_GPUTransferBuffer* transferBuffer;

export SDL_GPUTexture* testTexture;

/**
* this function is just for 2d texture and voxel slices. 
* \param layerCount should be 1 if not 2d array
*/
SDL_GPUTexture* uploadTexture(SDL_GPUDevice* gpu_device, SDL_GPUCopyPass* copyPass, Uint32 width, Uint32 height, Uint32 layerCount, Uint32 offset){
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "  - registering texture info");
    SDL_GPUTextureCreateInfo textureInfo{
        .type = (layerCount>1)? SDL_GPU_TEXTURETYPE_2D_ARRAY:SDL_GPU_TEXTURETYPE_2D,
        .format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
        .usage = SDL_GPU_TEXTUREUSAGE_SAMPLER,
        .width = width,
        .height = height,
        .layer_count_or_depth = layerCount,
        .num_levels = 1,
        .sample_count = SDL_GPU_SAMPLECOUNT_1,
        .props = 0,
    };
    SDL_GPUTexture* texture = SDL_CreateGPUTexture(gpu_device, &textureInfo);

    if(texture == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "texture register creation failed");
    }

    SDL_GPUTextureTransferInfo textureTransferInfo{
        .transfer_buffer = transferBuffer,
        .offset = offset,
    };
    
    SDL_GPUTextureRegion destination{
        .texture = texture,
        .layer = 0, // the first layer to transfer
        .w = width,
        .h = height,
        .d = 1 // can only be 1 except 3D texture
    };

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "  - uploading texture info");
    SDL_UploadToGPUTexture(copyPass, &textureTransferInfo, &destination, false);
    return texture;
}


//this function should only call during copy pass
export void addGPUTextures(SDL_GPUCopyPass* copyPass, SDL_GPUDevice* gpu_device){
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "  - Creating texture transfer buffer");
    SDL_GPUTransferBufferCreateInfo transferBufferInfo{
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = sizeof(testCube) // replace this with a loop that check through all the textures
    };
    transferBuffer = SDL_CreateGPUTransferBuffer(gpu_device, &transferBufferInfo);

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "  - transfering texture data");
    SDL_Color* transferSession =  (SDL_Color*)SDL_MapGPUTransferBuffer(gpu_device, transferBuffer, false);

    SDL_memcpy(transferSession, testCube, sizeof(testCube));//for loop as well

    SDL_UnmapGPUTransferBuffer(gpu_device, transferBuffer);
    SDL_ReleaseGPUTransferBuffer(gpu_device, transferBuffer);

    
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "  - bind data to texture");
    //upload texture info now
    testTexture = uploadTexture(gpu_device, copyPass, atlasSize.x, atlasSize.y, zLayerCount, 0);
    return;
}

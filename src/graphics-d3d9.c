#include <d3d9.h>
#include "graphics.h"

extern HWND win_return_hwnd();

static LPDIRECT3D9 g_d3d = NULL;
static LPDIRECT3DDEVICE9 g_d3dDevice = NULL;
static LPDIRECT3DVERTEXBUFFER9 g_d3dVB = NULL;

typedef struct CUSTOMVERTEX {
    FLOAT x, y, z, rhw;
    DWORD color;
} CUSTOMVERTEX;

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)


static bool init_d3d(){
    HWND hWnd = win_return_hwnd();
    g_d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if(g_d3d == NULL){
        printf("g_d3d is NULL\n");
        return FALSE;
    }
    D3DPRESENT_PARAMETERS d3dpp = {0};
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    HRESULT result = g_d3d->lpVtbl->CreateDevice(g_d3d, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_d3dDevice);
    if(result != D3D_OK){
        printf("CreateDevice failed\n");
        return FALSE;
    }
    return TRUE;
}

static bool init_vb(){
    CUSTOMVERTEX vertices[] = {
        { 250.0f, 250.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 255, 0)},
        {  50.0f, 250.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 0, 255)},
        {  50.0f,  50.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 0, 0)},


        { 250.0f, 50.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 255, 255)},
        {  250.0f,  250.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 255, 0)},
        {  50.0f, 50.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 0, 0)},

    };
    HRESULT result = g_d3dDevice->lpVtbl->CreateVertexBuffer(g_d3dDevice, 6 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_d3dVB, NULL);
    if(result != D3D_OK){
        printf("CreateVertexBuffer failed\n");
        return FALSE;
    }
    void* pVertices;
    g_d3dVB->lpVtbl->Lock(g_d3dVB, 0, sizeof(vertices), (void**)&pVertices, 9);
    CopyMemory(pVertices, vertices, sizeof(vertices));
    g_d3dVB->lpVtbl->Unlock(g_d3dVB);
    return TRUE;
}

bool gfx_init(){
    bool result = init_d3d();
    if(result == FALSE) return result;
    return init_vb();
}

bool gfx_resize(size_t width, size_t height){
    D3DVIEWPORT9 viewData = { 0, 0, width, height, 0.0f, 1.0f };
    g_d3dDevice->lpVtbl->SetViewport(g_d3dDevice, &viewData); // TODO: error checking
    return TRUE;
}

bool gfx_draw_triangle(){
    g_d3dDevice->lpVtbl->Clear(g_d3dDevice, 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    g_d3dDevice->lpVtbl->BeginScene(g_d3dDevice);
    g_d3dDevice->lpVtbl->SetStreamSource(g_d3dDevice, 0, g_d3dVB, 0, sizeof(CUSTOMVERTEX));
    g_d3dDevice->lpVtbl->SetFVF(g_d3dDevice, D3DFVF_CUSTOMVERTEX);
    g_d3dDevice->lpVtbl->DrawPrimitive(g_d3dDevice, D3DPT_TRIANGLELIST, 0, 2);
    g_d3dDevice->lpVtbl->EndScene(g_d3dDevice);

    g_d3dDevice->lpVtbl->Present(g_d3dDevice, NULL, NULL, NULL, NULL);
    return TRUE;
}


bool gfx_destroy(){
    if(g_d3dVB != NULL){
        g_d3dVB->lpVtbl->Release(g_d3dVB);
    }
    if(g_d3dDevice != NULL){
        g_d3dDevice->lpVtbl->Release(g_d3dDevice);
    }
    if(g_d3d != NULL){
        g_d3d->lpVtbl->Release(g_d3d);
    }
    return TRUE;
}

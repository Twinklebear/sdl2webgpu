#include "sdl2webgpu.h"
#include <SDL.h>
#include <SDL_syswm.h>

#include <webgpu/webgpu.h>

#ifdef SDL_VIDEO_DRIVER_COCOA
#include <Cocoa/Cocoa.h>
#include <QuartzCore/CAMetalLayer.h>
#endif

WGPUSurface sdl2GetWGPUSurface(WGPUInstance instance, SDL_Window *window)
{
#if defined(__EMSCRIPTEN__)
    WGPUEmscriptenSurfaceSourceCanvasHTMLSelector native_surface = {0};
    native_surface.chain.sType = WGPUSType_EmscriptenSurfaceSourceCanvasHTMLSelector;
    native_surface.selector.data = "#canvas";
    native_surface.selector.length = 7;
#else
    SDL_SysWMinfo wm_info;
    SDL_VERSION(&wm_info.version);
    SDL_GetWindowWMInfo(window, &wm_info);
#if defined(SDL_VIDEO_DRIVER_WINDOWS)
    WGPUSurfaceSourceWindowsHWND native_surface = {0};
    native_surface.chain.sType = WGPUSType_SurfaceSourceWindowsHWND;
    native_surface.hwnd = wm_info.info.win.window;
    native_surface.hinstance = wm_info.info.win.hinstance;
#elif defined(SDL_VIDEO_DRIVER_COCOA)
    id metal_layer = [CAMetalLayer layer];
    NSWindow *ns_window = wm_info.info.cocoa.window;
    [ns_window.contentView setWantsLayer:YES];
    [ns_window.contentView setLayer:metal_layer];

    WGPUSurfaceSourceMetalLayer native_surface = {0};
    native_surface.chain.sType = WGPUSType_SurfaceSourceMetalLayer;
    native_surface.layer = metal_layer;

#elif defined(SDL_VIDEO_DRIVER_X11)
    WGPUSurfaceSourceXlibWindow native_surface = {0};
    native_surface.chain.sType = WGPUSType_SurfaceSourceXlibWindow;
    native_surface.display = wm_info.info.x11.display;
    native_surface.window = wm_info.info.x11.window;
#elif defined(SDL_VIDEO_DRIVER_WAYLAND)
    WGPUSurfaceSourceWaylandSurface native_surface = {0};
    native_surface.chain.sType = WGPUSType_SurfaceSourceWaylandSurface;
    native_surface.display = wm_info.info.wl.display;
    native_surface.surface = wm_info.info.wl.surface;
#else
#error "Unhandled native platform"
#endif
#endif

    WGPUSurfaceDescriptor surface_desc = {};
    surface_desc.nextInChain = &native_surface.chain;
    return wgpuInstanceCreateSurface(instance, &surface_desc);
}

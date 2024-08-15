*This file does not render well, so it might be easier to read the raw markdown*
# Software overview:

## Project file structure:
  
project/  
│  
├── src/  
│   └── main.cpp  
│  
├── Application/  
│   ├── Application.h  
│   ├── Application.cpp  
│   │  
│   ├── Core_Utilities/  
│   │   └── buffer.h  
│   │  
│   ├── Engine/  
│   │   ├── engine.h  
│   │   ├── engine.cpp  
│   │   │  
│   │   ├── Scene/  
│   │   ├── Renderer/  
│   │   └── Game_logic/  
│   │  
│   └── Window/  
│       ├── window.h  
│       ├── window.cpp  
│       ├── gui.h  
│       ├── gui.cpp  
│       ├── implot/  
│       └── imgui/  
│  
└── CMakeLists.txt <-- Handels fetching of required dependencies using fetchcontent   


# TODO:  
## Implement tiled rendering  

# Understanding of the Tyler tiled homogeneous renderer  
Example of a tiled renderer that manages to achieve good performance.    
https://github.com/NotCamelCase/Tyler  

## Rasterizer  
Looking at the code for PipelineThread  

### Datastructures  
~ RenderEngine* m_pRenderEngine  
  - Framebuffers, vertexbuffers, indexbuffers, bound shader programs and holds all the pipeline threads  
~ RasterizerConfig m_RenderConfig   
  - Num threads, max primitives per iteration, tile size  
~ std::atomic<ThreadStatus> m_CurrentState   
  - Holds the state of each worker thread  
~ VertexCache  
  - Caches processed vertexes  
~ DrawParams m_ActiveDrawParams  
  - Start and end of the current primitives to be processed  

### Functions  
#### ProcessDrawcall()  
~ Starts the rendering process  
~ Loops over all the primitives it has been assigned through DrawParams and:  
    1. Runs the vertex shader on all the primitives -> returns the vertexes in clip space  
    2. Runs the clipper on the vertexes in clip space and assigns a bounding box to the triangle  
       - Will remove all triangles outside the view frustum  
    3. Runs a triangle setup and cull function on the triangle  
       - Will remove triangles if back-face culled  
    4. Binns all triangles into tiles  

***This will be done for all primitives, followed by stalling until all other threads are finished***  
~ It then executes the rasterizer    
~ Followed by executing the fragment shader  

#### ExecuteVertexShader(drawIdx, primIdx, pV0Clip, pV1Clip, pV2Clip)  
@param drawIdx --- Assigned primitives index which will only be used to fetch indices  
@param primIdx --- Primitive index relative to current iteration  
@param pV0Clip --- Pointer to the first vertex of the processed triangle output  
@param pV1Clip --- Pointer to the second vertex of the processed triangle output  
@param pV2Clip --- Pointer to the third vertex of the processed triangle output  
  
***I will not include the vertex cache for now***  

~ Retrieves the input vertexes from a common vertexbuffer based on  
    1. vertexStride  
    2. indexBuffer, which is indexed with   
        i.  vertexOffset --- constant, assuming this is related to the packing of vertexes, normals and uvs  
        ii. drawIdx  
    The equation becomes   
    *vertexBufferIndex = vertexStride * indexBuffer[vertexOffset + (3 * drawIdx + x)]*  
    where x depends on if it is the first, second or third vertex being accessed  
~ Instansiates the VS (vertex shader) program that is bound to the engine on each of the vertexes,  
  and assigns it to each of the output vertexes, now correctly in clip space  


#### ExecuteFullTriangleClipping(primIdx, v0Clip, v1Clip, v2Clip, pBbox)  
@param bbox --- A boinding box struct signifying the bounding box of the clipped triangle  

~ Tests whether or not the triangle is a trivial accept or trivial reject when clipping against the view frustum.  
  The following comment signifies the relationship between the view frustum and vertexes in clip space:  
    // Clip-space positions are to be bounded by:  
    // -w < x < w   -> LEFT/RIGHT  
    // -w < y < w   -> TOP/BOTTOM  
    //  0 < z < w   -> NEAR/FAR  

~ If it is a trivial accept, then compute the bounding box with ComputeBoundingBox function and assign the pBbox variable to this  
~ If it is a trivial reject, cull the triangle  
~ If it intesects the view frustum, then create a bounding box equal to the screen dimensions (overly conservative).   
  Can use Blinn's method of screen coverage calculation  

#### ComputeBoundingBox(v0Clip, v1Clip, v2Clip, width, height)  
~ Computes the NDC vertices for the triangle by doing perspective devide (v.x/v.w, v.y/v.w)  
~ Convertes NDC to raster space (width * (ndc.x + 1) * 0.5, height * (ndc.y + 1) * 0.5)  
~ Finds the min and max x and y positions for all the raster space vertexes  
~ Instansiates the bounding box with those values  

#### ExecuteTriangleSetupAndCull(primIdx, v0Clip, v1Clip, v2Clip)  
~ Transforms clip-space (x, y, z, w) vertices to device-space 2D homogeneous coordinates (hom_x, hom_y, z, w)  
    *TO_HOMOGENEOUS: glm::vec4(width * (v_clip.x + v_clip.w) * 0.5, height * (v_clip.y + v_clip.w) * 0.5, v_clip.z, v_clip.w)*  
~ Calculates matrix coefficients for the edge coefficients later used in the rasterization step   
        float a0 = (v2Homogen.y * v1Homogen.w) - (v1Homogen.y * v2Homogen.w);  
        float a1 = (v0Homogen.y * v2Homogen.w) - (v2Homogen.y * v0Homogen.w);  
        float a2 = (v1Homogen.y * v0Homogen.w) - (v0Homogen.y * v1Homogen.w);  

        float b0 = (v1Homogen.x * v2Homogen.w) - (v2Homogen.x * v1Homogen.w);  
        float b1 = (v2Homogen.x * v0Homogen.w) - (v0Homogen.x * v2Homogen.w);  
        float b2 = (v0Homogen.x * v1Homogen.w) - (v1Homogen.x * v0Homogen.w);  

        float c0 = (v2Homogen.x * v1Homogen.y) - (v1Homogen.x * v2Homogen.y);  
        float c1 = (v0Homogen.x * v2Homogen.y) - (v2Homogen.x * v0Homogen.y);  
        float c2 = (v1Homogen.x * v0Homogen.y) - (v0Homogen.x * v1Homogen.y);  
~ Interpolates the coefficients to get the determinant of the "vertex matrix" used for back-face culling  
        // det(M) == 0 -> degenerate/zero-area triangle  
        // det(M) < 0  -> back-facing triangle  
        float detM = (c0 * v0Homogen.w) + (c1 * v1Homogen.w) + (c2 * v2Homogen.w);  
~ Assigns the edge coefficients a0-2, b0-2 and c0-2 and stores the interpolated clip-space z deltas that will be used for perspective-correct  
  interpolation of the z-value later in the rasterization step  
~ returns detM > 0.0f as it represents whether or not the triangle should be ommitted due to back-face culling  

#### ExecuteBinner(primIdx, bbox)  
~   



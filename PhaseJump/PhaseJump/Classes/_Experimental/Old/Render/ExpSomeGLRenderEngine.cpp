#include <stdio.h>
//
// void SomeGLRenderEngine::RenderLineLoop(VectorList<float> const& vertices)
//{
//    RenderInternal([] () {
//        //    VertexPointer(2, GL_FLOAT, 0, vertices);
//        //    DrawArrays(GL_LINE_LOOP, 0, numVertices);
//    });
//}
//
// void SomeGLRenderEngine::RenderLine(VectorList<float> const& vertices)
//{
//    RenderInternal([] () {
//        //    VertexPointer(2, GL_FLOAT, 0, &vertices);
//        //    DrawArrays(GL_LINE_STRIP, 0, numVertices);
//    });
//}

//
//
//  SomeGLRenderEngine.mm
//  Phase Jump
//
//  Copyright 2010 Coin Bump. All rights reserved.
//
//
// #include "pch.h"
//
// #include "SomeGLRenderEngine.h"
// #include "PJ_GLHeaders.h"
// #include "PJ_Math.h"
// #include "PJ_DrawTexture.h"
// #include "PJ_Types.h"
//
// PJ_RenderStateStack    pjREStateStack;
// SomeGLRenderEngine*    PJ::renderEngine = NULL;    // Render engine.
// PJ_RenderState        PJ::renderState;
//
// PJ_REMatrixStack
// PJ::renderMatrixStack[static_cast<int>(MatrixStack::GLNumRenderStacks)]; int
// PJ::frameRenderCount = 0;
//
// SomeGLRenderEngine::SomeGLRenderEngine()
//:    mRenderScale(PJ_Vector2(1.0, 1.0)),
//    mRenderRotate(0),
//    mDefaultRenderer(new Renderer)
//{
//    mPixelCoordScale = PJ_Vector2(1, 1);
//    mTexCAEnabled = false;
//    mVertexAEnabled = false;
//    mColorAEnabled = false;
//    mRenderAlpha = 1.0;
//    mRenderAlpha2 = 1.0;
//
//    mRenderer = mDefaultRenderer;
//    mWhiteColor = PJ_FColor::white;
//}
//
///*
//    PushRenderState
//
// */
// void SomeGLRenderEngine::PushRenderState() {
//    pjREStateStack.push(PJ::renderState);
//
//    MatrixStack    curStack = PJ::renderState.mActiveMatrixStack;
//
//    SetActiveMatrixStack(MatrixStack::GLProjectionStack);
//    PushMatrix();
//    SetActiveMatrixStack(MatrixStack::GLModelViewStack);
//    PushMatrix();
//
//    SetActiveMatrixStack(curStack);    // Restore original.
//
//}
//
///*
//    Pops the render state.
//
//    Does not pop bound textures (expensive).
//
// */
// void SomeGLRenderEngine::PopRenderState() {
//    if (pjREStateStack.empty()) { return; }
//
//    PJ_RenderState    newState = pjREStateStack.top();
//    EnableScissorTest(newState.mScissorTestEnabled);
//    BindFrameBuffer(newState.mFrameBufferBound);
//    SetViewport(newState.mViewportX, newState.mViewportY,
//    newState.mViewportWidth, newState.mViewportHeight);
//    EnableBlend(newState.mBlendEnabled);
//    SetRenderMode(newState.mRenderMode);
//
//    SetActiveMatrixStack(MatrixStack::GLProjectionStack);
//    PopMatrix(true);
//    SetActiveMatrixStack(MatrixStack::GLModelViewStack);
//    PopMatrix(true);
//
//    // Restore current matrix stack.
//    SetActiveMatrixStack(newState.mActiveMatrixStack);
//
//    pjREStateStack.pop();
//}
//
// GLuint SomeGLRenderEngine::FrameBufferId()
//{
//    return PJ::renderState.mFrameBufferBound;
//}
//
// void SomeGLRenderEngine::SetRenderColorRGBA(PJ_Color color)
//{
//    PJ_FColor    fColor = PJ_ColorToFColor(color);
//    SetRenderColor(fColor);
//}
//
// void SomeGLRenderEngine::SetRenderColorRGB(PJ_Color color)
//{
//    // FUTURE: eliminate unnecessary state changes.
//    PJ_FColor    fColor = PJ_ColorToFColor(color);
//    fColor.alpha = 1.0f;
//    SetRenderColor(fColor);
//}
//
// void SomeGLRenderEngine::SetRenderColorRGBF(float red, float green, float
// blue)
//{
//    PJ_FColor    fColor(red, green, blue, 1.0);
//    SetRenderColor(fColor);
//}
//
// void SomeGLRenderEngine::SetRenderColorRGBAF(float red, float green, float
// blue, float alpha)
//{
//    SetRenderColor(PJ_FColor(red, green, blue, alpha));
//}
//
// void SomeGLRenderEngine::Render2DQuad(ZQuad const& v)
//{
//    Render2DTriangleStrip(&(*(const_cast<ZQuad*>(&v)))[0], 4);
//}
//
// void SomeGLRenderEngine::Render2DTriangleStrip(void* vertices, int
// numVertices)
//{
//    PreRender();
//
//    // EXPERIMENTAL CODE:
//    /*
//         DOESN'T WORK:
//
//         Doesn't seem to work without a depth buffer, depth buffer doesn't
//         support alpha blended polygons.
//
//     */
////#define __EXPERIMENTAL_23D__
// #ifdef __DEBUG__
// #ifdef __EXPERIMENTAL_23D__
////    vector<PJ_Vector3>    v3;
////    float* pVertices = static_cast<float*>(vertices);
////    for (int i = 0; i < numVertices; i++) {
////        PJ_Vector3 v;
////        v[0] = *pVertices; pVertices++;
////        v[1] = *pVertices; pVertices++;
////        v[2] = m2DZ;
////        v3.push_back(v);
////    }
////    VertexPointer(3, GL_FLOAT, 0, &v3[0]);
////    DrawArrays(GL_TRIANGLE_STRIP, 0, numVertices);
////    return;
// #endif
// #endif
//
//     VertexPointer(2, GL_FLOAT, 0, vertices);
//     DrawArrays(GL_TRIANGLE_STRIP, 0, numVertices);
//
//     // For tracking optimizations.
//     PJ::frameRenderCount++;
// }
//
// void SomeGLRenderEngine::Render2DTriangleFan(void* vertices, int numVertices)
//{
//     PreRender();
//
//     VertexPointer(2, GL_FLOAT, 0, vertices);
//     DrawArrays(GL_TRIANGLE_FAN, 0, numVertices);
// }
//
//
///*
//
//    CHECKLIST: this assumes that the appropriate glElements vertices, etc. are
//    already bound.
//
// */
// void SomeGLRenderEngine::RenderElementsTriangleStrip(GLsizei count, const
// GLvoid *indices)
//{
//    // IMPORTANT: the elements must be pointed to elsewhere.
//    PreRender();
//    glDrawElements(GL_TRIANGLE_STRIP, count, GL_UNSIGNED_SHORT, indices);
//
//}
//
// void SomeGLRenderEngine::SetActiveMatrixStack(MatrixStack renderStack)
//{
//    PJ::renderState.mActiveMatrixStack = renderStack;
//
//}
//
// #pragma mark - MATRICES
//
// void SomeGLRenderEngine::LoadIdentity()
//{
//    PJ::renderMatrixStack[static_cast<int>(PJ::renderState.mActiveMatrixStack)].LoadIdentity();
//    LoadMatrix();
//
//}
//
///*
//    LoadOrtho
//
//    __WIN_PHASE__: near, far are reserved keywords
//
// */
// void SomeGLRenderEngine::LoadOrtho(float left, float right, float bottom,
// float top, float _near, float _far)
//{
//    PJ::renderMatrixStack[static_cast<int>(PJ::renderState.mActiveMatrixStack)].mMatrix.LoadOrtho(left,
//    right, bottom, top, _near, _far); LoadMatrix();
//
//}
//
// void SomeGLRenderEngine::LoadPerspective(float fov_radians, float aspect,
// float zNear, float zFar)
//{
//    PJ::renderMatrixStack[static_cast<int>(PJ::renderState.mActiveMatrixStack)].mMatrix.LoadPerspective(fov_radians,
//    aspect, zNear, zFar); LoadMatrix();
//}
//
// void SomeGLRenderEngine::SetActiveMatrix(PJ_Matrix4& matrix, bool load)
//{
//    PJ::renderMatrixStack[static_cast<int>(PJ::renderState.mActiveMatrixStack)].mMatrix
//    = matrix; if (load) {
//        LoadMatrix();
//    }
//}
//
// void SomeGLRenderEngine::RotateAxisRad(Axis3D axis, float radians, bool load)
//{
//    PJ_Matrix4    rotateMatrix;
//
//    switch (axis) {
//        case Axis3D::AxisX:
//            rotateMatrix.LoadXRadRotation(radians);
//            break;
//        case Axis3D::AxisY:
//            rotateMatrix.LoadYRadRotation(radians);
//            break;
//        default:
//            rotateMatrix.LoadZRadRotation(radians);
//            break;
//    }
//
//    PJ::renderMatrixStack[static_cast<int>(PJ::renderState.mActiveMatrixStack)].mMatrix
//    =
//    PJ::renderMatrixStack[static_cast<int>(PJ::renderState.mActiveMatrixStack)].mMatrix
//    * rotateMatrix; if (load) {
//        LoadMatrix();
//    }
//
//}
//
// void SomeGLRenderEngine::RotateAxisDeg(Axis3D axis, float degrees, bool load)
//{
//    RotateAxisRad(axis, DEGREES_TO_RADIANSF(degrees), load);
//
//}
//
// void SomeGLRenderEngine::Translate(float x, float y, float z, bool load)
//{
//    if (0 == x && 0 == y && 0 == z) { return; }
//
//    PJ_Vector3    v(x, y, z);
//    PJ_Matrix4    matrix;
//    matrix.LoadTranslation(v);
//
//    PJ::renderMatrixStack[static_cast<int>(PJ::renderState.mActiveMatrixStack)].mMatrix
//    =
//    PJ::renderMatrixStack[static_cast<int>(PJ::renderState.mActiveMatrixStack)].mMatrix
//    * matrix; if (load) {
//        LoadMatrix();
//    }
//
//}
//
// void SomeGLRenderEngine::Scale(float x, float y, float z, bool load)
//{
//    PJ_Vector3    v(x, y, z);
//    PJ_Matrix4    matrix;
//    matrix.LoadScale(v);
//
//    PJ::renderMatrixStack[static_cast<int>(PJ::renderState.mActiveMatrixStack)].mMatrix
//    =
//    PJ::renderMatrixStack[static_cast<int>(PJ::renderState.mActiveMatrixStack)].mMatrix
//    * matrix; if (load) {
//        LoadMatrix();
//    }
//
//}
//
// PJ_Matrix4 SomeGLRenderEngine::GetActiveMatrix() const {
//    return
//    PJ::renderMatrixStack[static_cast<int>(PJ::renderState.mActiveMatrixStack)].mMatrix;
//
//}
//
// void SomeGLRenderEngine::SetBlendFunc(GLenum source, GLenum dest)
//{
//    PJ::renderState.SetBlendFunc(source, dest);
//}
//
// void SomeGLRenderEngine::BlendFuncDefault()
//{
//    SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//}
//
// void SomeGLRenderEngine::PushMatrix()
//{
//    PJ::renderMatrixStack[static_cast<int>(PJ::renderState.mActiveMatrixStack)].push(PJ::renderMatrixStack[static_cast<int>(PJ::renderState.mActiveMatrixStack)].mMatrix);
//
//}
//
// void SomeGLRenderEngine::PopMatrix(bool load)
//{
//    if
//    (PJ::renderMatrixStack[static_cast<int>(PJ::renderState.mActiveMatrixStack)].size()
//    > 1) {
//
//        PJ::renderMatrixStack[static_cast<int>(PJ::renderState.mActiveMatrixStack)].mMatrix
//        =
//        PJ::renderMatrixStack[static_cast<int>(PJ::renderState.mActiveMatrixStack)].top();
//        PJ::renderMatrixStack[static_cast<int>(PJ::renderState.mActiveMatrixStack)].pop();
//
//        if (load) {
//            LoadMatrix();
//        }
//    }
//
//}
//
// #pragma mark - SomeGLRenderEngine::Renderer
//
// PJ_FColor& SomeGLRenderEngine::GetRenderColor() {
//    if (mRenderColors.empty()) {
//        mRenderColors.push_back(PJ_FColor::white);
//    }
//    return mRenderColors[0];
//}
//
// PJ_FColor& SomeGLRenderEngine::GetRenderColorAt(size_t index) {
//    if (index < mRenderColors.size()) {
//        return mRenderColors[index];
//    }
//    return mWhiteColor;
//}
//
// void SomeGLRenderEngine::Renderer::SetRenderColor(PJ_FColor const& color) {
//    if (PJ::renderEngine->mRenderColors.empty()) {
//        PJ::renderEngine->mRenderColors.push_back(color);
//    }
//    else {
//        PJ::renderEngine->mRenderColors[0] = color;
//    }
//}
//
// void SomeGLRenderEngine::Renderer::RenderTexture(PJ_DrawTexture* tex, ZQuad
// const& v) {
//    PJ::renderEngine->BindTexture2D(tex->GetRenderID());
//    PJ::renderEngine->Render2DQuad(v);
//}
//
///*
//    WORKSPACE
//
//    Pseudocode below. Not part of the working project. Ignore.
//
// */
// #if FALSE
//
//
////#define __EXPERIMENTAL_3D__
// #ifdef __EXPERIMENTAL_3D__
//
// void PJ::renderEngine->Render2DTriangleStrip(void* _vertices, int
// numVertices)
//{
//     PreRender();
//
//     int        totalValues = numVertices*3;
//     float    *vertices = (float*) malloc(totalValues*sizeof(float));
//     int    index = 0;
//     for (int i = 0; i < totalValues; i += 3) {
//         vertices[i]=    ((float*)_vertices)[index];
//         vertices[i+1]=    ((float*)_vertices)[index+1];
//         vertices[i+2] = -1;
//         index += 2;
//     }
//
//     PJ::renderEngine->VertexPointer(3, GL_FLOAT, 0, vertices);
//     PJ::renderEngine->DrawArrays(GL_TRIANGLE_STRIP, 0, numVertices);
//
//     free(vertices);
//
// }
//
// #else
// #endif
//
//// FUTURE: support this (currently unsupported as a matrix operation).
////void pjRE::Rotate3Rad(float radians, float x, float y, float z, bool load)
////{
////    PJ_Matrix4    rotateMatrix;
////    PJ_Vector3    v(x, y, z);
////    rotateMatrix.Load3RadRotation(radians, v);
////    PJ::renderMatrixStack[PJ::renderState.mActiveMatrixStack].mMatrix =
/// PJ::renderMatrixStack[PJ::renderState.mActiveMatrixStack].mMatrix *
/// rotateMatrix; /    if (load) { /        PJ::renderEngine->LoadMatrix(); / }
////
////}
//
// #endif

//        GLuint          frameBufferBound;
//        bool            isFrameBufferBound;
//        bool            hasViewport;
//        GLint           viewportX;
//        GLint           viewportY;
//        GLsizei         viewportWidth;
//        GLsizei          viewportHeight;
//        bool            isBlendEnabled;
////        MatrixStack      mActiveMatrixStack;
//
////        typedef map<GLenum, GLuint>    TextureBoundMap;
////        TextureBoundMap    mTexturesBound;
//
//        GLRenderState();
//
//        void InvalidateFrameBuffer(GLuint bufferID);
//        bool BindFrameBuffer(GLuint bufferID);
//
//        void InvalidateTexture(GLuint textureID);
//        bool BindTexture(GLuint textureID, GLenum textureUnit);
//
//        bool SetRenderMode(PJ_RenderMode const& mode);
//        bool SetViewport(GLint x, GLint y, GLsizei width, GLsizei height);
//        bool EnableBlend(bool enable);
//        bool SetBlendFunc(GLenum sfactor, GLenum dfactor);
//
//        void PreRender();
//        Color RenderColor();
//        Color RenderColorAt(size_t index);

/*
 Renderer

 Use the renderer object to support switching in a batch renderer.
 This makes auto-batching of textures easier.

 */
//        class Renderer : public PJ_Renderer {
//        public:
//            // OVERRIDE:
//            virtual void SetRenderColor(Color const& color);
//            virtual void RenderTexture(PJ_DrawTexture* tex, ZQuad const& v);

//        };
//        PTR(PJ_Renderer)    mRenderer;    // OPTIONAL: encapsulates batch/no
//        batch renders. PTR(PJ_Renderer)    mDefaultRenderer;

//        GLuint FrameBuffer();
//
//        void Render(ZQuad const& v);
//        void RenderTriangleStrip2D(void* vertices, int numVertices);

//        void SetActiveMatrixStack(MatrixStack renderStack);
//        void PushRenderState();
//        void PopRenderState();
//
//        /*
//         Matrix stacks.
//
//         Currently, the matrix stacks are separate from the render state. If
//         it seems better to combine these in the future, feel free to do so.
//         Can't think of a good reason right now.
//
//         */
//        virtual void PushMatrix();
//        virtual void PopMatrix(bool load);
//
//        void SetBlendFunc(GLenum source, GLenum dest);
//        void BlendFuncDefault();
//
//        // OPTIONAL OVERRIDE:
//        virtual void Validate() {}

//        virtual void SetRenderMode(PJ_RenderMode const& mode) = 0;
//
//        // TEXTURES:
//        virtual void BindTexture2D(GLuint texture, GLenum textureUnit =
//        0x84C0) = 0;    // GL_TEXTURE0: 0x84c0
//
//        // MATRICES:
//        PJ_Matrix4 GetActiveMatrix() const;
//        void SetActiveMatrix(PJ_Matrix4& matrix, bool load);
//        void RotateAxisRad(Axis3D axis, float radians, bool load);
//        void RotateAxisDeg(Axis3D axis, float degrees, bool load);
//        void Translate(float x, float y, float z, bool load);
//        void Scale(float x, float y, float z, bool load);
//        void LoadIdentity();
//        void LoadOrtho(float left, float right, float bottom, float top, float
//        _near, float _far); void LoadPerspective(float fov_radians, float
//        aspect, float zNear, float zFar);
//
//        virtual void SetRenderAlpha(float alpha) { mRenderAlpha = alpha; } //
//        For interpolate factor type shaders void SetRenderColorRGBA(PJ_Color
//        color); void SetRenderColorRGB(PJ_Color color); void
//        SetRenderColorRGBF(float red, float green, float blue); void
//        SetRenderColorRGBAF(float red, float green, float blue, float alpha);
//        void SetRenderColor(Color const& fcolor) {
//        mRenderer->SetRenderColor(fcolor); }
//

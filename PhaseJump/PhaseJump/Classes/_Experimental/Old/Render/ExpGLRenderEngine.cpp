#include "GLHeaders.h"
#include "GLRenderEngine.h"
#include <stdio.h>

using namespace PJ;

// GLRenderEngine::GLRenderEngine()
//{
//     PJ::renderEngine = this;
//     pjGL2RenderEngine = this;
//
//     mMatrixLocation = -1;    // MUST define this for shader.
//     mCurProgram = NULL;
//
//     mUsingGL2 = true;
//     mNumMatrixStacks = 2;    // Default: use projection and model view
//     stacks.
// }
//
// void GLRenderEngine::LoadMatrix()
//{
//     // FUTURE: make this more efficient. Try to reduce matrix state changes.
// #ifdef __DEBUG__
//     if (mMatrixLocation < 0) {
////        PJ::Log("ERROR. Matrix location in shader must be defined.");
//    }
//
//    if (static_cast<int>(PJ::renderState.mActiveMatrixStack) < 0 ||
//    static_cast<int>(PJ::renderState.mActiveMatrixStack) > 1) {
//        assert(false);    // Make sure we don't get garbage access.
//    }
// #endif
//
//    if (1 == mNumMatrixStacks) {
//        UniformMatrix4fv(mMatrixLocation, PJ::renderMatrixStack[0].mMatrix.m);
//    }
//    else {
//        PJ_Matrix4 renderMatrix = PJ::renderMatrixStack[0].mMatrix *
//        PJ::renderMatrixStack[1].mMatrix; UniformMatrix4fv(mMatrixLocation,
//        renderMatrix.m);
//    }
//}

// void GLRenderEngine::ColorPointer(GLint size, GLenum type, GLsizei stride,
// const GLvoid *pointer)
//{
//     // If the colors are floats, they should be normalized (from 0.0 to 1.0).
//     if (GL_FLOAT == type) {
//         glVertexAttribPointer((GLuint)VSAttributeLoc::VSAColorLoc, size,
//         type, false, stride, pointer);
//     }
//     // If the colors are non-floats, they need to be normalized.
//     else {
//         glVertexAttribPointer((GLuint)VSAttributeLoc::VSAColorLoc, size,
//         type, true, stride, pointer);
//     }
// }

// void GLRenderEngine::LoadShaders()
//{
//     // Load default shaders.
//     // IMPORTANT: each program should flush its shaders after compiling and
//     linking. PJ_GLShaderProgramClass::ShaderRegistry*    registry =
//     PJ_GLShaderProgramClass::GetRegistry(); for (auto& _i : *registry) {
//         PJ_GLShaderProgramClass*    _class =
//         static_cast<PJ_GLShaderProgramClass*>(_i.second.get());
//         PJ_GLShaderProgram*    program = _class->GetProgram();
//         if (NULL != program) {
//             program->LoadAndLink();
//         }
//     }
// }

//
///*
//    SetRenderMode
//
//    Each render mode is implemented as a shader program.
//
//    FUTURE: only load shaders that are used per-app.
//
// */
// void GLRenderEngine::SetRenderMode(PJ_RenderMode const& _mode)
//{
//    // Make any dynamic state changes before we change the render mode,
//    otherwise
//    // optimizing render engine doesn't switch states properly.
//    RenderBlend    blend = _mode.mBlend;
//    if (RenderBlend::BlendShader == blend)
//    {
//        if (NULL != mCurProgram) {
//            blend = mCurProgram->GetBlendMode();
//        }
//        else {
//            blend = RenderBlend::BlendDefault;
//        }
//    }
//    PJ_RenderMode    mode = _mode;
//    mode.mBlend = blend;
//
//    PJ_String renderMode = mode.mRenderMode;
//
//    // OPTIMIZE: Use to auto switch to fastest shader.
//    // TODO: problem: this doesn't work if the alpha is changed *after* the
//    render mode. if (kRenderModeTextureCopyOrMod == renderMode) {
//        if (1.0f == GetRenderColor().alpha) {
//            renderMode = kRenderModeTextureCopy;
//        }
//        else {
//            renderMode = kRenderModeTextureUMod;
//        }
//    }
//    mode.mRenderMode = renderMode;
//
//    if (!PJ::renderState.SetRenderMode(mode)) {
//        return;
//    }
//
//    switch (blend) {
//        case RenderBlend::BlendUnknown:
//            PJ::Log("ERROR. BlendUnknown is not a valid blend mode.");
//            EnableBlend(false);
//            break;
//        case RenderBlend::BlendNone:
//            EnableBlend(false);
//            break;
//        case RenderBlend::BlendDefault:
//            EnableBlend(true);
//            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//            break;
//        case RenderBlend::BlendTexture:
//            EnableBlend(true);
//            glBlendFunc(GL_TEXTURE_BLEND_SRC, GL_TEXTURE_BLEND_DEST);
//            break;
//        case RenderBlend::BlendFunc: {
//            EnableBlend(true);
//            GLenum sourceBlendFunc =
//            PJ::renderState.mRenderMode.mSourceBlendFunc; GLenum destBlendFunc
//            = PJ::renderState.mRenderMode.mDestBlendFunc;
//            glBlendFunc(sourceBlendFunc, destBlendFunc);
//            break;
//        }
//        case RenderBlend::BlendSrcOneMinusSrc:
//            EnableBlend(true);
//            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//            break;
//        case RenderBlend::BlendMultiply:
//            EnableBlend(true);
//            // NOTE: these blend modes appear to be equivalent.
//            glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
//            //SetBlendFunc(GL_ZERO, GL_SRC_COLOR);
//            break;
//        case RenderBlend::BlendCurrent:
//            // No changes.
//            break;
//        default:
//            break;
//    }
//
//    PJ_GLShaderProgramClass::ShaderRegistry* registry =
//    PJ_GLShaderProgramClass::GetRegistry(); PJ_ClassPtr _class; if
//    (registry->FindClass(mode.mRenderMode, _class)) {
//        PJ_GLShaderProgramClass*    shaderClass =
//        static_cast<PJ_GLShaderProgramClass*>(_class.get());
//        PJ_GLShaderProgram* program = shaderClass->GetProgram();
//        if (NULL != program) {
//            UseShaderProgram(program);
//        }
//    }
//}

//
// void GLRenderEngine::UseShaderProgram(PJ_GLShaderProgram *program)
//{
//    // Because some clients support multiple windows, each with their own
//    state changes,
//    // attempting to reduce unnecessary states changes causes unpredictable
//    flicker.
// #ifdef __MULTIPLE_CONTEXTS_PHASE__
//    if (true)
//    {
// #else
//    if (program != mCurProgram)
//    {
// #endif
//
//        if (mCurProgram) {
//            mCurProgram->Finished();
//        }
//
//        mCurProgram = program;
//        if (mCurProgram) {
//            mCurProgram->Use();
//
//            mMatrixLocation = glGetUniformLocation(mCurProgram->ID(),
//            "u_modelViewProjMatrix"); if (mMatrixLocation < 0) {
//                PJ::Log("ERROR. Matrix location undefined.");
//            }
//            LoadMatrix();
//        }
//    }
//
//}
//
///*
//    PreRender
//
//    IMPORTANT: this must be called before any render operation. This is
//    because state information (color, alpha) needs to be passed to the shader.
//    This state information is not passed to the shader until PreRender is
//    called.
//
// */
// void GLRenderEngine::PreRender()
//{
//    if (NULL != mCurProgram) {
//        mCurProgram->PreRender();
//    }
//}

// void GLRenderEngine::Go() {
//     LoadShaders();
//
//     // Set up default blending (or it won't get set in OpenGL because this is
//     default in PJ_RenderMode).
//     glBlendFunc(PJ::renderState.mRenderMode.mSourceBlendFunc,
//     PJ::renderState.mRenderMode.mDestBlendFunc);
// }
//
//
// void GLRenderEngine::BindTexture2D(GLuint texture, GLenum glTexture)
//{
//     if (PJ::renderState.BindTexture(texture, glTexture)) {
//         glActiveTexture(glTexture);
//         glBindTexture(GL_TEXTURE_2D, texture);
//     }
// }
//
// #pragma mark -
// #pragma mark pjGL2
//
// void pjGL2::UniformMatrix4fv(GLint location, const GLfloat* value)
//{
//     ((GLRenderEngine*)(PJ::renderEngine))->UniformMatrix4fv(location, value);
// }
//
// void pjGL2::SetNumMatrixStacks(int val)
//{
//     GLRenderEngine::mNumMatrixStacks = val;
//
// }
//

// texture_varyColor.vsh
// texture.vsh
// uniformColor.vsh
// varyColor.vsh

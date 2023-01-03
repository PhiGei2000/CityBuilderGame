#include "rendering/shadowBuffer.hpp"

#include "misc/configuration.hpp"

#include <iostream>

#include <GL/glew.h>

ShadowBuffer::ShadowBuffer() {
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D_ARRAY, depthMap);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, Configuration::SHADOW_BUFFER_WIDTH, Configuration::SHADOW_BUFFER_HEIGHT, Configuration::SHADOW_CASCADE_COUNT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    constexpr float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    const int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Error: Shadow buffer not complete" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShadowBuffer::~ShadowBuffer() {
    glDeleteTextures(1, &depthMap);

    glDeleteFramebuffers(1, &fbo);
}

void ShadowBuffer::use() const {
    glViewport(0, 0, Configuration::SHADOW_BUFFER_WIDTH, Configuration::SHADOW_BUFFER_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);

    glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowBuffer::bindTextures() const {
    glActiveTexture(GL_TEXTURE0 + depthMapOffset);
    glBindTexture(GL_TEXTURE_2D_ARRAY, depthMap);
}

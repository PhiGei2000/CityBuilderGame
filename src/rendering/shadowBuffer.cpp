#include "rendering/shadowBuffer.hpp"

#include "misc/configuration.hpp"

#include <GL/glew.h>

ShadowBuffer::ShadowBuffer() {
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(Configuration::SHADOW_CASCADE_COUNT, &depthMaps[0]);
    for (int i = 0; i < Configuration::SHADOW_CASCADE_COUNT; i++) {
        glBindTexture(GL_TEXTURE_2D, depthMaps[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Configuration::SHADOW_BUFFER_WIDTH, Configuration::SHADOW_BUFFER_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMaps[i], 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShadowBuffer::~ShadowBuffer() {
    glDeleteTextures(1, &depthMaps[0]);

    glDeleteFramebuffers(1, &fbo);
}

void ShadowBuffer::use(unsigned int mapIndex) const {
    glViewport(0, 0, Configuration::SHADOW_BUFFER_WIDTH, Configuration::SHADOW_BUFFER_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glBindTexture(GL_TEXTURE_2D, depthMaps[mapIndex]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMaps[mapIndex], 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowBuffer::bindTextures() const {
    const int offset = 4;
    for (int i = 0; i < Configuration::SHADOW_CASCADE_COUNT; i++) {
        glActiveTexture(GL_TEXTURE0 + offset + i);
        glBindTexture(GL_TEXTURE_2D, depthMaps[i]);
    }
}

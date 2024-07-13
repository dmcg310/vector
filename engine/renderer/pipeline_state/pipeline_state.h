#pragma once

class PipelineState {
public:
  virtual void SetBlendState(bool enable) = 0;
  virtual void SetDepthTest(bool enable) = 0;
  virtual void SetCullFace(bool enable) = 0;
  virtual ~PipelineState() = default;
};
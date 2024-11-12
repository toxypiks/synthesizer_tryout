typedef struct RayOutBuffer {
  size_t max_length;
  size_t global_frames_count;
  float* global_frames;
} RayOutBuffer;

RayOutBuffer create_ray_out_buffer(size_t size);
void clear_ray_out_buffer(RayOutBuffer buf);
void copy_to_ray_out_buffer(RayOutBuffer *buf, void *bufferData, unsigned int frames);

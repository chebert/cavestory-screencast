#ifndef CONFIG_H_
#define CONFIG_H_

namespace config {

enum GraphicsQuality {
   HIGH_QUALITY,
   ORIGINAL_QUALITY
};

GraphicsQuality getGraphicsQuality();

}

#endif // CONFIG_H_
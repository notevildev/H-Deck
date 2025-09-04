Universal TFT_eSPI compatible color class for any color-related properties in `UIStyle`.

TFT_eSPI uses a non-standard compressed color format (16-bit), which lead to some major frustrations when trying to make render something using a non-standard color. This class aims to help eliminate such headaches, by storing 24-bit colors and implicitly compressing them back down to the 16-bit format.

## #Properties

#### `private uint8_t red_`;
> Stores the uncompressed value of red color channel

#### `private uint8_t green_`
> Stores the uncompressed value of the green color channel

#### `private uint8_t blue_`
> Stores the uncompressed value of the blue color channel


## #Initializing
For your convenience, there are 3 (+1 redundant) different `UIColor` initializers, allowing for (most) standard color initialization methods.
```c++
// 24-bit hexadecimal
UIColor dark_gray =   UIColor((int)0x141517) 
UIColor light_gray =  UIColor((uint32_t)0x232527) 

// 16-bit hexadecimal
UIColor foreground =  UIColor((uint_16t)0xFC9F) 

// 3-color RGB
UIColor yellow =   UIColor(
	(uint8_t)0,   // red channel
	(uint8_t)255, // blue channel
	(uint8_t)255, // green channel
)
```


## #Callable Functions

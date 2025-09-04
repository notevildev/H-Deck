Universal component styling class.

For added convenience, there is a static `DEFAULT_STYLE` definition in `DefaultStyles.h`, that acts as a fallback for any components without explicit style assignments.

## #Properties
#### `UIColor foreground_`
> Foreground color for a component
> *Primarily controls the text color for components with text.

#### `UIColor background_`
> Background color for a component

#### `UIColor border_color_`
> Border color for components with visible borders

#### `uint8_t border_thickness_`
> Border stroke weight for components with visible borders

#### `uint8_t text_size_ = 1`
> Font size for components with text
> *Due to the nature of TFT_eSPI, this is actually a **size multiplier**, that is stacked on top of the library's built-in default font size (~8px). 

#### `UIBoxSpacing padding_`
> Padding for a component (space between Border & Content)


## #Initializing
Initialization is fairly self-explanatory, all properties are directly initialized in the same order they are defined,
```c++
UIStyle myStyle = UIStyle(
	(UIColor)foreground, // no pointers! intended to use inline initialization
	(UIColor)background, 
	(UIColor)border_color, 
	(uint8_t)border_thickness, 
	(uint8_t)text_size, 
	(UIBoxSpacing)padding
)
```


## #Callable Functions

#### `UIStyle HighContrast() const`
> Returns a new instance of `UIStyle` with the background and foreground colors inverted.
> *Useful for highlighting selectable `CONTROL` components.

#### `UIStyle* SetForeground(UIColor& color)`
> Changes the `foreground_` property of a `UIStyle` to `color`. 
> *Returns itself to allow for chaining setters.

#### `UIStyle* SetBackground(UIColor& color)`
> Changes the `background_` property of a `UIStyle` to `color`. 
> *Returns itself to allow for chaining setters.

#### `UIStyle* SetBorderColor(UIColor& color)`
> Changes the `border_color_` property of a `UIStyle` to `color`. 
> *Returns itself to allow for chaining setters.

#### `UIStyle* SetBorderThickness(uint8_t thickness)`
> Changes the `border_thickness_` property of a `UIStyle` to `thickness`. 
> *Returns itself to allow for chaining setters.

#### `UIStyle* SetTextSize(uint8_t text_size)`
> Changes the `text_size`_ property of a `UIStyle` to `text_size`. 
> *Returns itself to allow for chaining setters.

#### `UIStyle* SetPadding(UIBoxSpacing& padding)`
> Changes the `padding_` property of a `UIStyle` to `padding`. 
> *Returns itself to allow for chaining setters.


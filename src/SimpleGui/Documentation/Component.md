Base component class. All UI components should inherit from this class, as it includes the base framework that ultimately powers SimpleGui.

## #Properties 

#### `protected bool absolute_`
> Flag to track whether a component uses absolute or dynamic positioning.
> 
> If this flag is set to false and the component's parent is updated, it will automatically be relocated to the origin (0, 0) of it's parent's bounds.

#### `protected bool dirty_`
> Flag to track whether a component requires a graphical update.
> 
> When `Component.Render()` is called, this flag is checked to determine if the component is to be redrawn to the TFT. See `Component.Invalidate()` and `Component.isAbsolute()` for setting & getting respectively.

#### `UIPoint pos_`
> 2D Grid Coordinate representing the component's **absolute position** on the TFT.

#### `UIRect size_`
> 2D grid coordinate representing the size of the component's content body

#### `Observable<bool> focused_`
> Flag to track whether the component holds user focus on the TFT.
> 
> Intended to be checked in `Component.Draw()` to determine if `style_` or `focused_style` should be used for rendering.

#### `Component* parent_`
> If the component resides within a parent container, this variable should point to said parent.
> 
> Set to `nullptr` by default.
> *This value is automatically updated when calling `Container.AddChild()`

#### `UIStyle* style_`
> Points to the component's standard [UIStyle](https://github.com/rondotdll/H-Deck/blob/dev/src/SimpleGui/Documentation/UIStyle.md)
> 
> Points to `DEFAULT_STYLE` by default.

#### `UIStyle* focused_style_`
> Points to the component's focused [UIStyle](https://github.com/rondotdll/H-Deck/blob/dev/src/SimpleGui/Documentation/UIStyle.md), which is subbed in depending on the value of `Component.focused_`
> 
> Points to `DEFAULT_STYLE_FOCUSED` by default.

## #Initializing 
The base component class **is not intended to be used by itself**, but as a foundation for other graphical components to inherit from.

Use explicit initialization for any child components:
```cpp
// default constructor (doesn't require any parameters)
default Component() {
	// Observable properties require explicit initialization. 
	this->focused_ = Observable<bool>(this, false)
}

// only needed for explicit overrides
// only position, dimensions, and style require values
explicit Component(
	   (UIPoint) position,
	    (UIRect) dimensions,
	  (UIStyle*) style,
	  (UIStyle*) focused_style_, // = DEFAULT_STYLE_FOCUSED
	(Component*) parent, // = nullptr
)
```

## #Callable
#### `virtual component_type_t type() const`
> Static pseudo-property that tracks the component's type (`NORMAL`, `CONTROL`, or `CONTAINER`)

#### `virtual void Render(bool force = false)`;
> Trampoline function to call `Component.Draw()` if either `Component.dirty_` **or** `force` are true

#### `virtual UIRect GetRenderedSize() const`
> Override for components with odd scaling / rendering properties.
>  
>  *Intended to return the graphical size of a component on the screen, including borders, headers, footers, etc.

#### `void Invalidate()`
> Since `Component.dirty_` is protected, this function should be called to update it to `true`

#### `Component* absolute(bool enabled = true)`
> Since `Component.absolute_` is protected, this function should be used to update it's value.
> *Returns itself to allow for chaining setters.

#### `bool isDirty() const`
> Returns the value of `Component.dirty_`

#### `bool isAbsolute() const`
> Returns the value of `Component.absolute_`

#### `uint16_t width() const`
> Shorthand for `Component.size_.x`

#### `uint16_t height() const`
> Shorthand for `Component.size_.y`

#### `uint16_t x() const`
> Shorthand for `Component.pos_.x`

#### `uint16_t y() const`
> Shorthand for `Component.pos_.y`

#### `UIPoint get_center() const`
> Returns the **logical center point** of the component

#### `virtual std::vector<Component*> Children()`
> Empty function to satisfy compiler, see [Container](https://github.com/rondotdll/H-Deck/blob/dev/src/SimpleGui/Documentation/Container.md)

#### `Component* Focus(bool state = true)`
> Sets `Component.focused_` to `state`
> *Returns itself to allow for chaining setters.

#### `Component* Unfocus()`
> Sets `Component.focused_` to `false`
> *Returns itself to allow for chaining setters.

#### `Component* MoveIntoParentBounds()`
> If the Component has a parent, moves it to be within it's parent's bounds.
> *Returns itself to allow for chaining setters.

#### `Component* FillParent()`
> If the Component has a parent, resizes it to take up the full body of it's parent
> *Returns itself to allow for chaining setters.

#### `Component* AlignToParent(UIAlignment alignment)`
> If the Component has a parent, moves it within it's parent's area depending on the value of `alignment`
> *Returns itself to allow for chaining setters.

#### `Component* SetPos(int x, int y)`
> Shorthand for setting `Component.pos_.x` and `Component.pos_.y`, respectively
> *Returns itself to allow for chaining setters.

#### `Component* MovePos(int x, int y)`
> Similar to `Component.SetPos()`, but will add to `Component.pos_.x` and `Component.pos_.y`, respectively
> *Returns itself to allow for chaining setters.

#### `Component* SetSize(int w, int h)`
> Shorthand for setting `Component.size_.x` and `Component.size_.y`, respectively
> *Returns itself to allow for chaining setters.

#### `Component* SetParent(Component* parent)`
> Change the value of `Component.parent_` to `parent`
> *Returns itself to allow for chaining setters.*

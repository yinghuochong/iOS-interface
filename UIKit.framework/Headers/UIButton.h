//
//  UIButton.h
//  UIKit
//
//  Copyright (c) 2005-2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIControl.h>
#import <UIKit/UIGeometry.h>
#import <UIKit/UIStringDrawing.h>
#import <UIKit/UIKitDefines.h>

NS_ASSUME_NONNULL_BEGIN

@class UIImage, UIFont, UIColor, UIImageView, UILabel;

typedef NS_ENUM(NSInteger, UIButtonType) {
    UIButtonTypeCustom = 0,                         // no button type
    UIButtonTypeSystem NS_ENUM_AVAILABLE_IOS(7_0),  // standard system button

    UIButtonTypeDetailDisclosure, //带右箭头的详细按钮
    UIButtonTypeInfoLight, //亮色的信息按钮
    UIButtonTypeInfoDark, //暗色的信息按钮  没看出来啥区别
    UIButtonTypeContactAdd, //添加联系人按钮
    
    UIButtonTypeRoundedRect = UIButtonTypeSystem,   // Deprecated, use UIButtonTypeSystem instead
};

NS_CLASS_AVAILABLE_IOS(2_0) @interface UIButton : UIControl <NSCoding>
//类方法
+ (instancetype)buttonWithType:(UIButtonType)buttonType;

@property(nonatomic)          UIEdgeInsets contentEdgeInsets UI_APPEARANCE_SELECTOR; // default is UIEdgeInsetsZero. On tvOS 10 or later, default is nonzero except for custom buttons. 可以设置这个值来调整所有内容的位置（上右下左），可以为负值
@property(nonatomic)          UIEdgeInsets titleEdgeInsets;                // default is UIEdgeInsetsZero  和上面的类似设置title的位置
@property(nonatomic)          BOOL         reversesTitleShadowWhenHighlighted; // default is NO. if YES, shadow reverses to shift between engrave and emboss appearance  是否改变标题的阴影
@property(nonatomic)          UIEdgeInsets imageEdgeInsets;                // default is UIEdgeInsetsZero  和上面类似调整 iamge 的位置
@property(nonatomic)          BOOL         adjustsImageWhenHighlighted;    // default is YES. if YES, image is drawn darker when highlighted(pressed)  高亮时是否改变图片颜色
@property(nonatomic)          BOOL         adjustsImageWhenDisabled;       // default is YES. if YES, image is drawn lighter when disabled  禁用时是否改变图片颜色
@property(nonatomic)          BOOL         showsTouchWhenHighlighted __TVOS_PROHIBITED;      // default is NO. if YES, show a simple feedback (currently a glow) while highlighted    高亮时是否给一个反馈
@property(null_resettable, nonatomic,strong)   UIColor     *tintColor NS_AVAILABLE_IOS(5_0); // The tintColor is inherited through the superview hierarchy. See UIView for more information.
@property(nonatomic,readonly) UIButtonType buttonType;

// you can set the image, title color, title shadow color, and background image to use for each state. you can specify data
//根据不同的状态设置标题、 图片、标题颜色、标题阴影颜色、背景图等
// for a combined state by using the flags added together. in general, you should specify a value for the normal state to be used
//组合状态时 使用标记组合，一般情况下你应该设置normal状态的值
// by other states which don't have a custom value set

- (void)setTitle:(nullable NSString *)title forState:(UIControlState)state;                     // default is nil. title is assumed to be single line
- (void)setTitleColor:(nullable UIColor *)color forState:(UIControlState)state UI_APPEARANCE_SELECTOR; // default if nil. use opaque white
- (void)setTitleShadowColor:(nullable UIColor *)color forState:(UIControlState)state UI_APPEARANCE_SELECTOR; // default is nil. use 50% black
- (void)setImage:(nullable UIImage *)image forState:(UIControlState)state;                      // default is nil. should be same size if different for different states
- (void)setBackgroundImage:(nullable UIImage *)image forState:(UIControlState)state UI_APPEARANCE_SELECTOR; // default is nil
- (void)setAttributedTitle:(nullable NSAttributedString *)title forState:(UIControlState)state NS_AVAILABLE_IOS(6_0); // default is nil. title is assumed to be single line

//根据状态获取相应的值
- (nullable NSString *)titleForState:(UIControlState)state;          // these getters only take a single state value
- (nullable UIColor *)titleColorForState:(UIControlState)state;
- (nullable UIColor *)titleShadowColorForState:(UIControlState)state;
- (nullable UIImage *)imageForState:(UIControlState)state;
- (nullable UIImage *)backgroundImageForState:(UIControlState)state;
- (nullable NSAttributedString *)attributedTitleForState:(UIControlState)state NS_AVAILABLE_IOS(6_0);

// these are the values that will be used for the current state. you can also use these for overrides. a heuristic will be used to
// determine what image to choose based on the explict states set. For example, the 'normal' state value will be used for all states
// that don't have their own image defined.

@property(nullable, nonatomic,readonly,strong) NSString *currentTitle;             // normal/highlighted/selected/disabled. can return nil
@property(nonatomic,readonly,strong) UIColor  *currentTitleColor;        // normal/highlighted/selected/disabled. always returns non-nil. default is white(1,1)
@property(nullable, nonatomic,readonly,strong) UIColor  *currentTitleShadowColor;  // normal/highlighted/selected/disabled.
@property(nullable, nonatomic,readonly,strong) UIImage  *currentImage;             // normal/highlighted/selected/disabled. can return nil
@property(nullable, nonatomic,readonly,strong) UIImage  *currentBackgroundImage;   // normal/highlighted/selected/disabled. can return nil
@property(nullable, nonatomic,readonly,strong) NSAttributedString *currentAttributedTitle NS_AVAILABLE_IOS(6_0);  // normal/highlighted/selected/disabled. can return nil

// return title and image views. will always create them if necessary. always returns nil for system buttons
@property(nullable, nonatomic,readonly,strong) UILabel     *titleLabel NS_AVAILABLE_IOS(3_0);
@property(nullable, nonatomic,readonly,strong) UIImageView *imageView  NS_AVAILABLE_IOS(3_0);

// these return the rectangle for the background (assumes bounds), the content (image + title) and for the image and title separately. the content rect is calculated based
// on the title and image size and padding and then adjusted based on the control content alignment. there are no draw methods since the contents
// are rendered in separate subviews (UIImageView, UILabel)

- (CGRect)backgroundRectForBounds:(CGRect)bounds;
- (CGRect)contentRectForBounds:(CGRect)bounds;
- (CGRect)titleRectForContentRect:(CGRect)contentRect;
- (CGRect)imageRectForContentRect:(CGRect)contentRect;
@end

@interface UIButton(UIButtonDeprecated)

@property(nonatomic,strong) UIFont         *font              NS_DEPRECATED_IOS(2_0, 3_0) __TVOS_PROHIBITED;
@property(nonatomic)        NSLineBreakMode lineBreakMode     NS_DEPRECATED_IOS(2_0, 3_0) __TVOS_PROHIBITED;
@property(nonatomic)        CGSize          titleShadowOffset NS_DEPRECATED_IOS(2_0, 3_0) __TVOS_PROHIBITED;

@end

NS_ASSUME_NONNULL_END

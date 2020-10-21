//
//  MyVideoView.m
//  ClearVision
//
//  Created by ants-mac on 17/3/17.
//  Copyright © 2017年 www.yurfly.com. All rights reserved.
//

#import "VideoView.h"
@interface VideoView ()

/*
 * 云台操作8个方向的指示图标
 *
 * 1:左上 2:正上 3:右上
 * 4:正左 5:中间 6:正右
 * 7:左下 8:正下 9:右下
 */

@property (strong, nonatomic) UIImageView *i1;
@property (strong, nonatomic) UIImageView *i2;
@property (strong, nonatomic) UIImageView *i3;
@property (strong, nonatomic) UIImageView *i4;
@property (strong, nonatomic) UIImageView *i6;
@property (strong, nonatomic) UIImageView *i7;
@property (strong, nonatomic) UIImageView *i8;
@property (strong, nonatomic) UIImageView *i9;

@property (nonatomic,strong) UIActivityIndicatorView * indicatorView;

@end

@implementation VideoView

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        [self initialize];
    }
    return self;
}

-(void)initialize{
    
    NSLog(@"initialize----%s",__FUNCTION__);
    
    self.userInteractionEnabled = YES;
    //self.backgroundColor = PLAY_BG_color;
    self.backgroundColor = [UIColor blackColor];
        
    self.layer.borderColor = [[UIColor clearColor]CGColor]; //直接给layer加边框
    self.layer.borderWidth = 1.0f;
 
//    [self setValue:self.videoLayer forKey:@"layer"];
    
    //add by hq in 2017-7-13  将解码器和播放器加在播放画布上
//    self.h265decoder = [[H265Dec alloc]init];
//    self.playH265View = [[OpenGLView20 alloc]initWithFrame:self.bounds];
//    self.playH265View.hidden = YES;
//    [self addSubview:self.playH265View];
    
    self.videoLayer = [[VideoLayer alloc]init];
    self.videoLayer.frame = self.bounds;
    [self.layer addSublayer:self.videoLayer];
    
    self.button = [UIButton buttonWithType:UIButtonTypeCustom];
    [self.button setImage:[UIImage imageNamed:@"add"] forState:UIControlStateNormal];
    [self.button setImage:[UIImage imageNamed:@"add1"] forState:UIControlStateHighlighted];
    self.button.backgroundColor = [UIColor redColor];
    CGRect rect = self.frame;
    CGFloat btn_w = rect.size.width/3.0;
    CGFloat btn_h = btn_w;
    CGFloat btn_y = (rect.size.height-btn_h)/2.0;
    self.button.frame=CGRectMake(btn_w, btn_y, btn_w, btn_h);
        
    //ptz
    self.i1 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"leftup.png"]];
    self.i2 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"up.png"]];
    self.i3 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"rightup.png"]];
    self.i4 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"left.png"]];
    self.i6 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"right.png"]];
    self.i7 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"leftdown.png"]];
    self.i8 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"down.png"]];
    self.i9 = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"rightdown.png"]];
    [self addSubview:self.i1];
    [self addSubview:self.i2];
    [self addSubview:self.i3];
    [self addSubview:self.i4];
    [self addSubview:self.i6];
    [self addSubview:self.i7];
    [self addSubview:self.i8];
    [self addSubview:self.i9];
    self.i1.backgroundColor = self.i2.backgroundColor = self.i3.backgroundColor = self.i4.backgroundColor = self.i6.backgroundColor = self.i7.backgroundColor = self.i8.backgroundColor = self.i9.backgroundColor = [UIColor redColor];
    self.i1.hidden = YES;
    self.i2.hidden = YES;
    self.i3.hidden = YES;
    self.i4.hidden = YES;
    self.i6.hidden = YES;
    self.i7.hidden = YES;
    self.i8.hidden = YES;
    self.i9.hidden = YES;

    [self addSubview:self.indicatorView];
}

-(UIActivityIndicatorView *)indicatorView
{
    if(!_indicatorView){
        
        _indicatorView = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhite];
        
        _indicatorView.frame = self.bounds;
        
        [_indicatorView stopAnimating];
        
        _indicatorView.hidesWhenStopped = YES;
    }
    return _indicatorView;
}

-(void)showIndicatorView
{
    [self.indicatorView startAnimating];
    self.indicatorView.hidden = NO;
}

-(void)dismissIndicatorView
{
    [self.indicatorView stopAnimating];
    self.indicatorView.hidden = YES;
}

- (void)dealloc
{
    if(self.videoLayer)
    {
        [self.videoLayer UnInitDecoder];
    }
    
    NSLog(@"Videoview Dealloc-- %@ ",self);
}

-(void)layoutSublayersOfLayer:(CALayer *)layer
{
    //必须要重写这个方法？？？？

//    [super layoutSublayersOfLayer:layer];
    
    CGFloat w = 44, h = 44;
    self.i1.frame = CGRectMake(0, 0, w, h);
    self.i2.frame = CGRectMake((self.bounds.size.width-w)/2, 0, w, h);
    self.i3.frame = CGRectMake(self.bounds.size.width-w, 0, w, h);
    self.i4.frame = CGRectMake(0, (self.bounds.size.height-h)/2, w, h);
    self.i6.frame = CGRectMake(self.bounds.size.width-w, (self.bounds.size.height-h)/2, w, h);
    self.i7.frame = CGRectMake(0, self.bounds.size.height-h, w, h);
    self.i8.frame = CGRectMake((self.bounds.size.width-w)/2, self.bounds.size.height-h, w, h);
    self.i9.frame = CGRectMake(self.bounds.size.width-w, self.bounds.size.height-h, w, h);
    
    self.videoLayer.frame = self.bounds;

    self.indicatorView.frame = self.bounds;
}

/*
 * zoom操作指示图标
 */
- (void)zoomOut:(BOOL)isOut start:(BOOL)isStart
{
    if (isOut && isStart) {
        self.i9.image = [UIImage imageNamed:@"leftup.png"];
        self.i7.image = [UIImage imageNamed:@"rightup.png"];
        self.i3.image = [UIImage imageNamed:@"leftdown.png"];
        self.i1.image = [UIImage imageNamed:@"rightdown.png"];
    } else {
        self.i1.image = [UIImage imageNamed:@"leftup.png"];
        self.i3.image = [UIImage imageNamed:@"rightup.png"];
        self.i7.image = [UIImage imageNamed:@"leftdown.png"];
        self.i9.image = [UIImage imageNamed:@"rightdown.png"];
    }
    [self bringSubviewToFront:self.i1];
    [self bringSubviewToFront:self.i3];
    [self bringSubviewToFront:self.i7];
    [self bringSubviewToFront:self.i9];
    self.i1.hidden = !isStart;
    self.i3.hidden = !isStart;
    self.i7.hidden = !isStart;
    self.i9.hidden = !isStart;
}


-(BOOL)touchesShouldCancelInContentView:(UIView *)view{
    return YES;
}

-(void)showAllImages
{
    for (UIImageView * imageV in self.subviews) {
        
        if([imageV isKindOfClass:[UIImageView class]])
        {
            imageV.hidden = NO;
            [self bringSubviewToFront:imageV];
            
        }
    }
}

-(void)dismissAllImages
{
    for (UIImageView * imageV in self.subviews) {
        
        if([imageV isKindOfClass:[UIImageView class]])
        {
            imageV.hidden = YES;
            
        }
    }
}

@end

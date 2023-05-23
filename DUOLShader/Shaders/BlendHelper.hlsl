// PhotoShop의 Layer합성 관련 함수의 HLSL버전

#define BlendLinearDodgef             BlendAddf
#define BlendLinearBurnf             BlendSubstractf
#define BlendAddf(base, blend)         min(base + blend, 1.0)
#define BlendSubstractf(base, blend)     max(base + blend - 1.0, 0.0)
#define BlendLightenf(base, blend)         max(blend, base)
#define BlendDarkenf(base, blend)         min(blend, base)
#define BlendLinearLightf(base, blend)     (blend < 0.5 ? BlendLinearBurnf(base, (2.0 * blend)) : BlendLinearDodgef(base, (2.0 * (blend - 0.5))))
#define BlendScreenf(base, blend)         (1.0 - ((1.0 - base) * (1.0 - blend)))
#define BlendOverlayf(base, blend)     (base < 0.5 ? (2.0 * base * blend) : (1.0 - 2.0 * (1.0 - base) * (1.0 - blend)))
#define BlendSoftLightf(base, blend)     ((blend < 0.5) ? (2.0 * base * blend + base * base * (1.0 - 2.0 * blend)) : (sqrt(base) * (2.0 * blend - 1.0) + 2.0 * base * (1.0 - blend)))
#define BlendColorDodgef(base, blend)     ((blend == 1.0) ? blend : min(base / (1.0 - blend), 1.0))
#define BlendColorBurnf(base, blend)     ((blend == 0.0) ? blend : max((1.0 - ((1.0 - base) / blend)), 0.0))
#define BlendVividLightf(base, blend)     ((blend < 0.5) ? BlendColorBurnf(base, (2.0 * blend)) : BlendColorDodgef(base, (2.0 * (blend - 0.5))))
#define BlendPinLightf(base, blend)     ((blend < 0.5) ? BlendDarkenf(base, (2.0 * blend)) : BlendLightenf(base, (2.0 *(blend - 0.5))))
#define BlendHardMixf(base, blend)     ((BlendVividLightf(base, blend) < 0.5) ? 0.0 : 1.0)
#define BlendReflectf(base, blend)         ((blend == 1.0) ? blend : min(base * base / (1.0 - blend), 1.0))

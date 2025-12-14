/*
    Renders a linear gradient, with alpha
 */
Shader "PhaseJump/LinearGradientTransparent"
{
	Properties
	{
        _Color1 ("Color 1", Color) = (1.0, 0, 0, 1.0)
        _Color2 ("Color 2", Color) = (1.0, 0, 1.0, 1.0)
	}
    SubShader {
        Tags {"Queue"="Transparent" "RenderType"="Transparent"}
        LOD 100
        
        ZWrite Off
        Blend SrcAlpha OneMinusSrcAlpha
        
        Pass {
            CGPROGRAM
            
            #pragma vertex vert
            #pragma fragment frag
            
            fixed4 _Color1;
            fixed4 _Color2;
             
            struct vertIn {
                float4 vertex : POSITION;
                float2 texcoord : TEXCOORD0;
            };

            struct vertOut {
                float4 vertex : POSITION;
                float2 texcoord : TEXCOORD0;
            };

            vertOut vert(vertIn vi) {
                vertOut vo;
                vo.vertex = UnityObjectToClipPos(vi.vertex);
                vo.texcoord = vi.texcoord;
                return vo;
            }

            half4 frag(vertOut vi) : COLOR {
                float4 factor;
                factor = vi.texcoord.x;

                float4 color0Delta = _Color2[0] - _Color1[0];
                float4 color1Delta = _Color2[1] - _Color1[1];
                float4 color2Delta = _Color2[2] - _Color1[2];
                float4 color3Delta = _Color2[3] - _Color1[3];

                float4 color0 = _Color1[0] + color0Delta * factor;
                float4 color1 = _Color1[1] + color1Delta * factor;
                float4 color2 = _Color1[2] + color2Delta * factor;
                float4 color3 = _Color1[3] + color3Delta * factor;

                half4 result;
                result.r = color0;
                result.g = color1;
                result.b = color2;
                result.a = color3;
                
                return result;
            }
            ENDCG
        }
    }
}

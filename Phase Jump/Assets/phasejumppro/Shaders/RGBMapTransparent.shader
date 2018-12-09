/*
    Maps each color channel (R, G, B) to a new color
    Useful for quickly recoloring a texture that has 3 primary colors
 */
Shader "Phase Jump/RGBMapTransparent"
{
	Properties
	{
		_MainTex ("Texture", 2D) = "white" {}
        _RedColor ("RedColor", Color) = (1.0, 0, 0, 1.0)
        _GreenColor ("GreenColor", Color) = (0, 1.0, 0, 1.0)
        _BlueColor ("BlueColor", Color) = (0, 0, 1.0, 1.0)
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
            
            sampler2D _MainTex;
            fixed4 _RedColor;
            fixed4 _GreenColor;
            fixed4 _BlueColor;
             
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
                half4 texColor = tex2D(_MainTex, vi.texcoord);
                
                half4 r = _RedColor * texColor.r;
                half4 g = _GreenColor * texColor.g;
                half4 b = _BlueColor * texColor.b;
    
                half4 finalColor;
                finalColor.r = r.r+g.r+b.r;
                finalColor.g = r.g+g.g+b.g;
                finalColor.b = r.b+g.b+b.b;
                finalColor.a = texColor.a;
                
                return finalColor;
            }
            ENDCG
        }
    }
}

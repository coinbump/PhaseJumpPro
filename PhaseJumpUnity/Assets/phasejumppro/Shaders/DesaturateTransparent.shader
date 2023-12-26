/*
    Desaturates texture color by specified factor
 */
Shader "PhaseJump/DesaturateTransparent"
{
	Properties
	{
		_MainTex ("Texture", 2D) = "white" {}
        _Factor("Factor", Range(0.0,1.0)) = 1.0
	}
    SubShader {
        Tags {"Queue"="Transparent" "RenderType"="Transparent"}
        LOD 100
        
        ZWrite Off  // Off for transparent
        Blend SrcAlpha OneMinusSrcAlpha
        Lighting Off
        Cull Off    // To render rotated/flipped sprites

        Pass {
            CGPROGRAM
            
            #pragma vertex vert
            #pragma fragment frag
            
            sampler2D _MainTex;
            float _Factor;
             
            struct vertIn {
                float4 vertex : POSITION;
				float4 color : COLOR;
                float2 texcoord : TEXCOORD0;
            };

            struct vertOut {
                float4 vertex : POSITION;
				float4 color : COLOR;
                float2 texcoord : TEXCOORD0;
            };

            vertOut vert(vertIn vi) {
                vertOut vo;
                vo.vertex = UnityObjectToClipPos(vi.vertex);
                vo.texcoord = vi.texcoord;
				vo.color = vi.color;
                return vo;
            }

            float random (float2 uv)
            {
                return frac(sin(dot(uv,float2(12.9898,78.233)))*43758.5453123);
            }

            half4 frag(vertOut vi) : COLOR {
                half4 texColor = tex2D(_MainTex, vi.texcoord);
                float desatColor = (texColor.r*.3+texColor.g*.59+texColor.b*.11);
                half4 destColor = half4(desatColor, desatColor, desatColor, texColor.a);
                float factor = _Factor;
               
                half4 mixColor = lerp(texColor, destColor, _Factor);
                mixColor[3] = texColor[3] * random(vi.texcoord) * vi.color.a;
                return mixColor;
            }
            ENDCG
        }
    }
}

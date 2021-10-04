float4 main(PixelInputType input) : SV_TARGET
{
	return float4(sin(length(a) * 100.0) * 0.5 + 0.5, sin(a.y * 50.0), 0, 1);
	//return float4(0.5f, 1.0f, 1.0f, 1.0f);
}
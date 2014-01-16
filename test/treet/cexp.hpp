/* http://stackoverflow.com/a/19877048/778858 */


namespace cexp
{
	constexpr uint pow(uint base, uint exponent)
	{
		return (exponent == 0) ? 1 :
			   (exponent % 2 == 0) ?  pow(base, exponent/2)
									 *pow(base, exponent/2) :
			   base*pow(base, exponent-1);
	}
}
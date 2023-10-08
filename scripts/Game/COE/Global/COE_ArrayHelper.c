//------------------------------------------------------------------------------------------------
//! Same as SCR_ArrayHelperT, but for types T that do not support strong references
class COE_ArrayHelperT<Class T>
{
	//------------------------------------------------------------------------------------------------
	//! Shuffle an array
	//! \param items
	//! \param shuffles number of shuffle passes to happen. min 1, max 10
	//! @code
	//! array<int> values = { 1, 2, 3, 4, 5 };
	//! SCR_ArrayHelperT<int>.Shuffle(values);
	//! Print(values); // outputs e.g { 4, 1, 5, 2, 3 }
	//! @endcode
	// NATIVE version
	static void Shuffle(notnull inout array<T> items, int shuffles = 1)
	{
		if (items.Count() < 2)
			return;

		// two checks are faster than Math.ClampInt
		if (shuffles < 1)
			shuffles = 1;

		if (shuffles > 10)
			shuffles = 10;

		while (shuffles > 0)
		{
			for (int i = 0, count = items.Count(); i < count; i++)
			{
				int index1 = Math.RandomInt(0, count);
				int index2 = Math.RandomInt(0, count);
				if (index1 != index2)
					items.SwapItems(index1, index2);
			}

			shuffles--;
		}
	}
};

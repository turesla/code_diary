

void quickSort(int *a, int first, int last)
{	
	int max = sizeof(a) / sizeof(int);
	if(first >= max || last >= max || last <= first)
	{
		return;
	}
	int pivot = a[first];
	int left = first + 1;
	int right = last;
	int temp = 0;
	while(left <= right)
	{
		while(a[right] >= pivot && right >= left)
		{
			right--;
		}
		while(a[left] <= pivot && left <= right)
		{
			left++;
		}

		if(left >= right)
		{
			break;
		}

		temp = a[left];
		a[left] = a[right];
		a[right] = temp;
		left++, right++;
	}
	//a[first] = a[right];
	//a[right] = pivot;
	quickSort(a, first, right - 1);
	quickSort(a, left, last);
}
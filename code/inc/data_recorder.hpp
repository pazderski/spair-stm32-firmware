#pragma once

template < int size, class DataType >
class DataRecorder
{
	uint16_t sampleNumber;
public:
	DataType data[size];
	static const uint16_t buf_size = size;

	DataRecorder()
	{
		sampleNumber = 0;
	}

	void Start()
	{
		sampleNumber = 0;
	}

	bool Record(DataType d)
	{
		if (sampleNumber == size) {
			return false;
		}
		data[sampleNumber++] = d;
		return true;
	}

	void RecordCyclically(DataType d)
	{
		data[sampleNumber++] = d;
		if (sampleNumber == size) {
			sampleNumber = 0;
		}
	}

};

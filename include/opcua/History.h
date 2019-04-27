#pragma once
#include "opcua/ExtensionObject.h"
#include "opcua//DateTime.h"
#include "opcua/NodeId.h"
#include <vector>
#include "opcua/DataValue.h"

namespace OWA {
	namespace OpcUa {
		struct AggregateConfiguration
		{
			AggregateConfiguration();

			bool useSeverCapabilitiesDefaults;
			bool treatUncertainAsBad;
			uint8_t percentDataBad;
			uint8_t percentDataGood;
			bool useSlopedExtrapolation;
		};

		struct ReadRawModifiedDetails : public ExtensionObject
		{
			ReadRawModifiedDetails();

			bool			isReadModified;
			DateTime	startTime;
			DateTime	endTime;

			Counter		numValuesPerNode;
			bool			returnBounds;

			typedef std::shared_ptr <ReadRawModifiedDetails> Ptr;
		};

		struct ReadProcessedDetails : public ExtensionObject
		{
			ReadProcessedDetails();

			DateTime startTime;
			DateTime endTime;
			Duration processingInterval;
			std::vector<NodeId> aggregateTypes;
			AggregateConfiguration aggregateConfiguration;

			typedef std::shared_ptr <ReadProcessedDetails> Ptr;
		};

		struct HistoryData : public ExtensionObject
		{
			HistoryData();

			std::vector<DataValue> dataValues;
		};
	}
}

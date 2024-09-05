int main()
{
	 QByteArray tmpCboreAr;

	 QFile fileJsonCbor("cbortest.json");
	 if(fileJsonCbor.open(QIODevice::ReadOnly))
	 {
	 	QByteArray tmp = fileJsonCbor.readAll();
	 		qDebug() << "tmp" << tmp; //////////////////////////////////////
	 	QByteArray::iterator itb = tmp.begin(), ite = tmp.end();
	 	uint32_t fileSize = tmp.size();
	 	while(fileSize)
	 	{
	 		if(*itb == 0x5B) // '['
	 		{
	 			QByteArray::iterator itTmp = itb;
	 			uint8_t quantityArr = 1;
	 			uint16_t arSize = 0;
	 			while(quantityArr)
	 			{
	 				itTmp++;
	 				if(*itTmp == 0x5B) // array in array
	 				{
	 					quantityArr++;
	 				}
	 				if(*itTmp == 0x5D) // ']'
	 				{
	 					quantityArr--;
	 				}
	 				if(arSize == 0)
	 				{
	 					if((*itTmp == 0x5B) || (*itTmp == 0x7B) || ((*itTmp >= 0x30) && (*itTmp <= 0x39)) || (*itTmp == 0x2D))
	 					{
	 						arSize++;
	 					}
	 				}
	 				if(quantityArr == 1)
	 				{
	 					if(*itTmp == 0x2C) // ','
	 					{
	 						arSize++;
	 					}
	 				}
	 			}
	 			if(arSize < 24)
	 			{
	 				tmpCboreAr.push_back(0x80 | arSize); // set (ar type | ar size)
	 			}
	 			else if(arSize < 256)
	 			{
	 				tmpCboreAr.push_back(0x80 | 24);
	 				tmpCboreAr.push_back(arSize);
	 			}
	 			else if(arSize < 65535)
	 			{
	 				tmpCboreAr.push_back(0x80 | 25);
	 				tmpCboreAr.push_back(arSize >> 8);
	 				tmpCboreAr.push_back(arSize);
	 			}
	 		}
	 		else if(*itb == 0x7B) // '{'
	 		{
	 			QByteArray::iterator itTmp = itb;
	 			uint8_t quantityObj = 1;
	 			uint16_t objSize = 0;
	 			while(quantityObj)
	 			{
	 				itTmp++;
	 				if(*itTmp == 0x7B) // obj in obj
	 				{
	 					quantityObj++;
	 				}
	 				if(*itTmp == 0x7D) // '}'
	 				{
	 					quantityObj--;
	 				}
	 				if(quantityObj == 1)
	 				{
	 					if(*itTmp == 0x3A) // ':'
	 					{
	 						objSize++;
	 					}
	 				}
	 			}
	 			if(objSize < 24)
	 			{
	 				tmpCboreAr.push_back(0xA0 | objSize); // set (obj type | obj size)
	 			}
	 			else if(objSize < 256)
	 			{
	 				tmpCboreAr.push_back(0xA0 | 24);
	 				tmpCboreAr.push_back(objSize);
	 			}
	 			else if(objSize < 65535)
	 			{
	 				tmpCboreAr.push_back(0xA0 | 25);
	 				tmpCboreAr.push_back(objSize >> 8);
	 				tmpCboreAr.push_back(objSize);
	 			}
	 		}
	 		else if(*itb == 0x22) // '"'
	 		{
	 			itb++;
	 			QByteArray::iterator itTmp = itb;
	 			uint16_t textSize = 0;
	 			while(*itTmp != 0x22)
	 			{
	 				itTmp++;
	 				textSize++;
	 			}
	 			if(textSize < 24)
	 			{
	 				tmpCboreAr.push_back(0x60 | textSize); // set (string type | str size)
	 			}
	 			else if(textSize < 256)
	 			{
	 				tmpCboreAr.push_back(0x60 | 24);
	 				tmpCboreAr.push_back(textSize);
	 			}
	 			else if(textSize < 65535)
	 			{
	 				tmpCboreAr.push_back(0x60 | 25);
	 				tmpCboreAr.push_back(textSize >> 8);
	 				tmpCboreAr.push_back(textSize);
	 			}

	 			while(*itb != 0x22)
	 			{
	 				tmpCboreAr.push_back(*itb);
	 				itb++;
	 			}
	 		}
	 		else if(*itb >= 0x30 && *itb <= 0x39)
	 		{
	 			bool isNum = true;
	 			QByteArray tmpNum;
	 			while(isNum)
	 			{
	 				tmpNum.push_back(*itb);
	 				itb++;
	 				if(*itb >= 0x30 && *itb <= 0x39)
	 				{
	 					isNum = true;
	 				}
	 				else
	 				{
	 					isNum = false;
	 				}
	 			}
	 			uint32_t number = tmpNum.toUInt();

	 			if(number < 24)
	 			{
	 				tmpCboreAr.push_back(0x00 | number); // set (uint type | uint size)
	 			}
	 			else if(number < 256)
	 			{
	 				tmpCboreAr.push_back(0x00 | 24);
	 				tmpCboreAr.push_back(number);
	 			}
	 			else if(number < 65535)
	 			{
	 				tmpCboreAr.push_back(0x00 | 25);
	 				tmpCboreAr.push_back(number >> 8);
	 				tmpCboreAr.push_back(number);
	 			}
	 			else
	 			{
	 				tmpCboreAr.push_back(0x00 | 26);
	 				tmpCboreAr.push_back(number >> 16);
	 				tmpCboreAr.push_back(number >> 8);
	 				tmpCboreAr.push_back(number);
	 			}
	 		}
	 		else if(*itb == 0x2D) // '-'
	 		{
	 			itb++;
	 			if(*itb >= 0x30 && *itb <= 0x39)
	 			{
	 				bool isNum = true;
	 				QByteArray tmpNum;
	 				while(isNum)
	 				{
	 					tmpNum.push_back(*itb);
	 					itb++;
	 					if(*itb >= 0x30 && *itb <= 0x39)
	 					{
	 						isNum = true;
	 					}
	 					else
	 					{
	 						isNum = false;
	 					}
	 				}
	 				int32_t number = tmpNum.toUInt() - 1;

	 				if(number < 24)
	 				{
	 					tmpCboreAr.push_back(0x20 | number); // set (int type | uint size)
	 				}
	 				else if(number < 256)
	 				{
	 					tmpCboreAr.push_back(0x20 | 24);
	 					tmpCboreAr.push_back(number);
	 				}
	 				else if(number < 65535)
	 				{
	 					tmpCboreAr.push_back(0x20 | 25);
	 					tmpCboreAr.push_back(number >> 8);
	 					tmpCboreAr.push_back(number);
	 				}
	 				else
	 				{
	 					tmpCboreAr.push_back(0x20 | 26);
	 					tmpCboreAr.push_back(number >> 16);
	 					tmpCboreAr.push_back(number >> 8);
	 					tmpCboreAr.push_back(number);
	 				}
	 			}
	 		}

	 		itb++;
	 		fileSize--;
	 	}
	 }


	 return 0;
}
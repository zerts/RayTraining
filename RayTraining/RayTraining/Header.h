#pragma once

using namespace System;
using namespace System::Threading;

namespace SystemThreadingExample
{
	public ref class Work
	{
	public:
		void StartThreads()
		{
			// Start a thread that calls a parameterized static method.
			Thread^ newThread = gcnew
				Thread(gcnew ParameterizedThreadStart(Work::DoWork));
			newThread->Start(42);

			// Start a thread that calls a parameterized instance method.
			Work^ someWork = gcnew Work;
			newThread = gcnew Thread(
				gcnew ParameterizedThreadStart(someWork,
					&Work::DoMoreWork));
			newThread->Start("The answer.");
		}

		static void DoWork(Object^ data)
		{
			Console::WriteLine("Static thread procedure. Data='{0}'",
				data);
		}

		void DoMoreWork(Object^ data)
		{
			Console::WriteLine("Instance thread procedure. Data='{0}'",
				data);
		}
	};
}
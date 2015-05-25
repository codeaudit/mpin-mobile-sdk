using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Core;

namespace MPinSDK.Common
{
    public static class UIDispatcher
    {
        private static CoreDispatcher dispatcher;

        public static void Initialize(CoreDispatcher coreDispatcher)
        {
            UIDispatcher.dispatcher = coreDispatcher;
        }

        public static void Execute(Action action)
        {
            InnerExecute(action).Wait();
        }

        private static async Task InnerExecute(Action action)
        {
            if (dispatcher.HasThreadAccess)
                action();
            else
            {
                if (dispatcher.HasThreadAccess)
                    action();

                else await dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => action());
            }
        }
    }

}

﻿namespace Sitecore.LiveTesting
{
  using System;
  using System.Configuration;
  using System.IO;
  using System.Reflection;

  /// <summary>
  /// Defines the base class for tests.
  /// </summary>
  [DynamicConstruction]
  public class LiveTest : ContextBoundObject
  {
    /// <summary>
    /// The default application id.
    /// </summary>
    private const string DefaultApplicationId = "Sitecore.LiveTesting.Default";

    /// <summary>
    /// The website path setting name.
    /// </summary>
    private const string WebsitePathSettingName = "Sitecore.LiveTesting.WebsitePath";

    /// <summary>
    /// The name of method that gets default application manager.
    /// </summary>
    private const string GetDefaultTestApplicationManagerName = "GetDefaultTestApplicationManager";

    /// <summary>
    /// The name of method that gets default application host.
    /// </summary>
    private const string GetDefaultApplicationHostName = "GetDefaultApplicationHost";

    /// <summary>
    /// The default test application manager.
    /// </summary>
    private static readonly TestApplicationManager DefaultTestApplicationManager = new TestApplicationManager();

    /// <summary>
    /// Gets default test application manager for the specified test type.
    /// </summary>
    /// <param name="testType">The test type.</param>
    /// <returns>An instance of <see cref="DefaultTestApplicationManager"/>.</returns>
    public static TestApplicationManager GetDefaultTestApplicationManager(Type testType)
    {
      return DefaultTestApplicationManager;
    }

    /// <summary>
    /// Gets default application host for the specified test type.
    /// </summary>
    /// <param name="testType">Type of the test.</param>
    /// <returns>The default application host.</returns>
    public static ApplicationHost GetDefaultApplicationHost(Type testType)
    {
      return new ApplicationHost(DefaultApplicationId, "/", ConfigurationManager.AppSettings.Get(WebsitePathSettingName) ?? Directory.GetParent(Environment.CurrentDirectory).FullName);
    }

    /// <summary>
    /// Creates an instance of corresponding class.
    /// </summary>
    /// <param name="testType">Type of the test to instantiate.</param>
    /// <returns>Instance of the class.</returns>
    public static LiveTest Instantiate(Type testType)
    {
      MethodInfo getDefaultTestApplicationManagerMethod = Utility.GetInheritedMethod(testType, GetDefaultTestApplicationManagerName, new[] { typeof(Type) });
      MethodInfo getDefaultApplicationHostMethod = Utility.GetInheritedMethod(testType, GetDefaultApplicationHostName, new[] { typeof(Type) });

      if (getDefaultTestApplicationManagerMethod == null)
      {
        throw new InvalidOperationException(string.Format("Cannot create an instance of type '{0}' because there is no '{1}' static method defined in its inheritance hierarchy. See '{2}' methods for an example of corresponding method signature.", testType.FullName, GetDefaultTestApplicationManagerName, typeof(LiveTest).FullName));
      }

      if (getDefaultApplicationHostMethod == null)
      {
        throw new InvalidOperationException(string.Format("Cannot create an instance of type '{0}' because there is no '{1}' static method defined in its inheritance hierarchy. See '{2}' methods for an example of corresponding method signature.", testType.FullName, GetDefaultApplicationHostName, typeof(LiveTest).FullName));
      }

      object[] typeArguments = { testType };

      TestApplicationManager testApplicationManager = (TestApplicationManager)getDefaultTestApplicationManagerMethod.Invoke(null, typeArguments);

      if (testApplicationManager == null)
      {
        throw new InvalidOperationException(string.Format("Failed to get an instance of '{0}'.", typeof(TestApplicationManager).FullName));
      }

      ApplicationHost host = (ApplicationHost)getDefaultApplicationHostMethod.Invoke(null, typeArguments);

      if (host == null)
      {
        throw new InvalidOperationException(string.Format("Failed to get an instance of '{0}'.", typeof(ApplicationHost).FullName));
      }

      TestApplication testApplication = testApplicationManager.StartApplication(host);

      if (testApplication == null)
      {
        throw new InvalidOperationException("Failed to get application to execute tests in.");
      }

      return (LiveTest)testApplication.CreateObject(testType);
    }
  }
}

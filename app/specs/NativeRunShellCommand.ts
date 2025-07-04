import type {TurboModule} from 'react-native';
import {TurboModuleRegistry} from 'react-native';

export interface Spec extends TurboModule {
  runAsync(command: string): Promise<string>;
  runSync(command: string): string;
}

export default TurboModuleRegistry.getEnforcing<Spec>('RunShellCommand');
